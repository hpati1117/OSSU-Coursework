#include <stdio.h>
#include <cs50.h>
#include <math.h>

//Function to print the total number of coins needed.
void CoinsNeeded(int);

int main(void){
    
    //Variables to hold the change the user inputs (floatChange) and
    //another to hold the change after being converted to an int (change).
    int change;
    float floatChange;
    
    //Asks user for a float greater than to 0.
    do{
    
    printf("How much change is there?: ");
    floatChange = get_float();
    
    }while (floatChange < 0);
    
    //Converts the float the user input into an int accurately.
    change = roundf(floatChange * 100);

    CoinsNeeded(change);
    
}

void CoinsNeeded(change){
    
    //Counter to keep track of the number of coins needed.
    int coinCount = 0;
    
    //Checks for quarters, then dimes, nickels, and pennies.
    while (change - 25 >= 0){
        
        change -= 25;
        coinCount++;
        
    }
    
    while (change - 10 >= 0){
        
        change -= 10;
        coinCount++;
        
    }
    
    while (change - 5 >= 0){
        
        change -= 5;
        coinCount++;
        
    }
    
    while (change - 1 >= 0){

        change -= 1;
        coinCount++;
        
    }
    
    //Prints out the minimum number of coins needed.
    printf("%d\n", coinCount);
    
}