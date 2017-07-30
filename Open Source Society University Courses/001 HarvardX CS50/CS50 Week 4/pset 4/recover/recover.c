#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



int main(int argc, char *argv[])
{
    // ensure the user entered 2 command-line args
    if (argc != 2)
    {
        // otherwise, return error 1
        fprintf(stderr, "Correct Usage: ./recover 'image'\n");
        return 1;
    }
    

    
    // open and ensure the file entered is valid
    FILE *cardFile = fopen(argv[1], "r");
    if (cardFile == NULL)
    {
        // otherwise return error 2
        fprintf(stderr, "Error occured opening the file given\n");
        return 2;
    }
    

    
    // reserve memory to be used for each block itterated through
    unsigned char *currentBlock = (unsigned char*)malloc(512);
    
    // values to hold the number of the file we're currently on, the
    // file name, whether we're currently writing an image, variable
    // to store the new file, and an end of file int, respectively.
    int fileNumber = 0;
    char fileName[8];
    bool writingJPG = false;
    FILE *newFile;
    int endOfFile = fread(currentBlock, 512, 1, cardFile);
    
    
    
    // while not at the end of the file...
    while (endOfFile > 0){
        
        // if the start of the block indicates it's a new image...
        if (currentBlock[0] == 0xff && currentBlock[1] == 0xd8 &&
            currentBlock[2] == 0xff && (currentBlock[3] & 0xf0) == 0xe0)
        {
        
            // ...and we're not currently writing to an image...
            if (!(writingJPG))
            {
                // create a new file appropriately named
                sprintf(fileName, "%03i.jpg", fileNumber);
                
                // open the new file made in write mode
                newFile = fopen(fileName, "w");
                
                // change the state of writingJPG to true
                writingJPG = true;
                
            }
            
            // ...otherwise, if we are currently writing on an image...
            else
            {
                //close the current image
                fclose(newFile);
                
                // change the state of writingJPG to false
                writingJPG = false;
                
                // increase the file number by 1 for the next file name 
                // to be incremented
                fileNumber++;
                
                // create a new file appropriately named
                sprintf(fileName, "%03i.jpg", fileNumber);
                
                // open the new file made in write mode
                newFile = fopen(fileName, "w");
                
                // change the state of writingJPG to true
                writingJPG = true;
            }
            
            // write the block to the new file
            fwrite(currentBlock, 512, 1, newFile);
        }
        
        // else if we're currently writting to a file...
        else if (writingJPG)
        {
            // write the block to the new file
            fwrite(currentBlock, 512, 1, newFile);
        }
        
        // read 512 bytes in the cardFile and store returned value
        // in endOfFile, which will become 0 once the end is reached.
        endOfFile = fread(currentBlock, 512, 1, cardFile);
    }
    

    
    // close the file and free memory
    fclose(cardFile);
    free(currentBlock);
    
    // success
    return 0;
}