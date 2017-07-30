#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void)
{
    
    FILE *inptr = fopen("large", "r");
    if (inptr == NULL)
    {
        
        printf("error opening file\n");
        fclose(inptr);
        return 2;
        
    }
    
    char word[45];
    int letterCount[27];
    
    for (int i = 0; i < 27; i++)
    {
        
        letterCount[i] = 0;
        
    }
    
    while (fscanf(inptr, "%s", word) != EOF)
    {
        
        if (word[0] == 39)
        {
            
            letterCount[26] += 1;
            
        }
        else
        {
            
            letterCount[word[0] - 97] += 1;
            
        }
        
    }
    
    for (int i = 0; i < 27; i++)
    {
        
        printf("The letter \'%c\' appears %i times\n", (char)i + 97, letterCount[i]);
        
    }
    
}