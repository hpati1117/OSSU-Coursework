#include <cs50.h>
#include <stdio.h>

int main(void){
    
    char letter = get_char();
    
    switch (letter){
        
        case 'y':
        case 'Y':
            printf("Yes!\n");
            break;
            
        case 'n':
        case 'N':
            printf("No!\n");
            break;
            
        default:
            printf("!Error\n");
            break;
        
    }
    
}