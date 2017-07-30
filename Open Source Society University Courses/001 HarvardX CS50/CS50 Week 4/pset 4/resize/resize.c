/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"



int main(int argc, char *argv[])
{
    // ensure user entered 3 args (#, infile, outfile)
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }



    // remembers and ensures the resize factor is between 1-100
    int resizeBy = atoi(argv[1]);
    if (!(resizeBy >= 1 && resizeBy <= 100))
    {
        fprintf(stderr, "Number to resize by must be between 1-100\n");
        return 2;
    }



    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];



    // open input file and return an error if one occurs
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }



    // open output file and return error if one occurs
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }



    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);



    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    
    
    // create variables to store the appropriate info
    int oldWidth, oldHeight, oldSizeImage, 
        oldSize, oldPadding, newPadding;
    
    
    
    // stores the old and new bi.biWidth
    oldWidth = bi.biWidth;
    bi.biWidth = oldWidth * resizeBy;
    
    // stores the old and new bi.biHeight
    oldHeight = bi.biHeight;
    bi.biHeight = oldHeight * resizeBy;
    
    // stores the old and new padding
    oldPadding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // stores the old and new bi.biSizeImage
    oldSizeImage = bi.biSizeImage;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) +
                     newPadding) * abs(bi.biHeight);
                     
    // stores the old and new bf.bfSize
    oldSize = bf.bfSize;
    bf.bfSize = bi.biSizeImage
            + sizeof(BITMAPINFOHEADER) 
            + sizeof(BITMAPFILEHEADER);



    // write outfile's changed BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's changed BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    
    
    // get the height of the image and, for each scanline...
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        
        // go over the same line 'resizeBy' times.
        for (int n = 0; n < resizeBy; n++)
        {
            
            // move the cursor to start of line...
            fseek(inptr, 
            bf.bfOffBits + (oldWidth * 3 + oldPadding) * i, 
            SEEK_SET);
            
            // for each pixel in the line...
            for (int j = 0; j < oldWidth; j++)
            {
                
                // store the value of the pixel here soon...
                RGBTRIPLE triple;
        
                // read the rgb triple and store it...
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // write the pixel 'resizeBy' times.
                for (int m = 0; m < resizeBy; m++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // add padding
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
            
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
