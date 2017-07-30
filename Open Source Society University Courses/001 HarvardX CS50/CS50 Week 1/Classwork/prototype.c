#include <cs50.h>
#include <stdio.h>

void cough(int);

int main(void){
    
    printf("How many times should I cough? ");
    
    int number = get_int();
    
    cough(number);
    
}

void cough(int number_of_times){
    
    for (int i = 0; i < number_of_times; i++){
        
        printf("cough\n");
        
    }
    
}