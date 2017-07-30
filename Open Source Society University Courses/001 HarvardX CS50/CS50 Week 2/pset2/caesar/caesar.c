#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]){
    
    //Returns an error if the number of args entered isn't 2.
    if (argc != 2){
        
        printf("Please enter a non-negative integer for the key\n");
        return 1;
        
    }
    
    //Convert the arg entered by the user into an int.
    int key = atoi(argv[1]);
    
    //Gets the plaintext from the user.
    printf("plaintext: ");
    string plaintext = get_string();
    
    //Prints the ciphertext by...
    printf("ciphertext: ");
    
    //Itterating through each character in the plaintext...
    for (int place = 0, textLength = strlen(plaintext); place < textLength; place++){
        
        //Check if the current character being itterated through is a letter...
        if (isalpha(plaintext[place])){
            
            //If it is a letter, check if it's lowercase. If it is...
            if (islower(plaintext[place])){
                
                //print the ciphered lowercase character.
                printf("%c", (((plaintext[place] - 97) + key) % 26) + 97);
                
            }
            
            //Otherwise, print the uppercase character.
            else{
                
                printf("%c", (((plaintext[place] - 65) + key) % 26) + 65);
                
            }
            
        }
        
        //When the character isn't a letter, just prints out the character.
        else{
            
            printf("%c", plaintext[place]);
            
        }
    }
    
    //Print a newline.
    printf("\n");
    
}