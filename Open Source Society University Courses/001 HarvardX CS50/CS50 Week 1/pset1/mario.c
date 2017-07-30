#include <stdio.h>
#include <cs50.h>

//Function to print out the pyramid.
void printPyramid(int);

int main(void){
    
    int height;
    
    //Gets a height between 0 and 23, inclusive.
    do{
        
        printf("Enter a height between 0 and 23: ");
        height = get_int();
        
    }while(height < 0 || height > 23);
    
    printPyramid(height);
    
}

void printPyramid(height){
    
    //Creates the initial number of spaces and hashes needed.
    int spaces = height - 1;
    int hashes = 1;
    
    //Creates variables used as counters to keep count of the number of
    //spaces (s) and hashes (h) that should be printed out on the current
    //line.
    int s, h;
    
    while(height > 0){
        
        spaces = height - 1;
        
        for (s = spaces; s != 0; s--){
            
            printf(" ");
            
        }
        
        for (h = hashes; h != 0; h--){
        
            printf("#");
            
        }
        
        //To make this work with the easy mario assignment, simply 
        //turn the code between here...
        printf("  ");
        
        for (h = hashes; h != 0; h--){
        
            printf("#");
            
        }//...and here into a comment.
        
        
        printf("\n");
        spaces--;
        hashes++;
        height--;
    
    }
    
}