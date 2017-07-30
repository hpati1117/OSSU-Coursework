#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]){
    
    //Creates a string variable to use later. Made now to avoid 
    //'Undeclared identifier' error.
    string keyphrase;
    
    //Returns an error if the number of args entered isn't 2. Otherwise...
    if (argc != 2){
        
        printf("Please enter a non-negative integer for the key\n");
        return 1;
        
    }
    else{
        
        //Assign keyphrase to be the user-entered keyphrase.
        keyphrase = argv[1];
        
        //Itterates through eack character in 'keyphrase'...
        for (int i = 0; keyphrase[i]; i++){
            
            //Converting the character to lowercase...
            keyphrase[i] = tolower(keyphrase[i]);
            
            //And ensuring the character is alphabetical. Otherwise,
            //we return 1 and end the program.
            if (!(isalpha(keyphrase[i]))){
            
                printf("Please enter a non-negative integer for the key\n");
                return 1;
             
            }
        
        }
        
    }
    
    //Gets the plaintext from the user.
    printf("plaintext: ");
    string plaintext = get_string();
    
    //Prints the ciphertext by...
    printf("ciphertext: ");

    /*
    Itterating through each character in the plaintext...
    
    Variables in for loop w/ purpose:
    
    all ints
    place: keep track of current place in plaintext
    keyPlace: keep track of current place in the keyphrase
    keyLength: stores the length of keyphrase
    textlength: stores the length of plaintext
    */
    for (int place = 0, keyPlace = 0, keyLength = strlen(keyphrase), textLength = strlen(plaintext); place < textLength; place++){
        
        //Check if the current character being itterated through is alphabetical...
        if (isalpha(plaintext[place])){
            
            //If the current index for the keyphrase is not in the actual
            //keyphrase, reset the keyPlace to 0 to begin anew.
            if (keyPlace >= keyLength){
                
                keyPlace = 0;
                
            }
            
            //Check if it's lowercase. If it is...
            if (islower(plaintext[place])){
                
                //print the ciphered lowercase character.
                printf("%c", (((plaintext[place] - 97) + (keyphrase[keyPlace] - 97)) % 26) + 97);
                keyPlace++;
                
            }
            
            //Otherwise, print the ciphered uppercase character.
            else{
                
                printf("%c", (((plaintext[place] - 65) + (keyphrase[keyPlace] - 97)) % 26) + 65);
                keyPlace++;
                
            }
            
        }
        
        //When the character isn't alphabetical, just prints out the character.
        else{
            
            printf("%c", plaintext[place]);
            
        }
    }
    
    //Print a newline.
    printf("\n");
    
}