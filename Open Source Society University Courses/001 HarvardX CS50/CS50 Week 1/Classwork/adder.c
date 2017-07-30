#include <cs50.h>
#include <stdio.h>

int main(void){
    
    printf("x is ");
    int x = get_int();
    
    printf("y is ");
    int y = get_int();
    
    printf("sum of x and y is %i\n", x + y);
    
    if (x + y <= 10){
        
        printf("the sum is not greater than 10\n");
        
    }
    else{
        
        printf("the sum is greater than 10\n!");
        
    }
}