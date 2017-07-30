#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>


int main(void){
    
    //Gets the user's name
    string name = get_string();
    
    //Creates a constant character that stays as an empty space.
    const string space = " ";
    
    //Makes a variable that points to the first set of characters
    //(persumably the first name) that are not a space.
    string listOfNames = strtok(name, space);
    
    //As long as the variable ListOfNames doesn't return NULL...
    while (listOfNames != NULL){
        
        //print out the first character, capitalized, in the name...
        printf("%c", toupper(listOfNames[0]));
        
        //then move on to the next name in the full name and repeat.
        listOfNames = strtok(NULL, space);
        
    }
    
    //Prints a newline.
    printf("\n");

}

//I love the strtok function now.