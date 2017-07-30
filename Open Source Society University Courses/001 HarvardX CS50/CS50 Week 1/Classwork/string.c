#include <stdio.h>
#include <cs50.h> 

int main(void){
    
    printf("Your name: ");
    
    string name = get_string();
    
    printf("\nHello, %s\n", name);
    
}