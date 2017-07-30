#include <stdio.h>
#include <cs50.h>

void BottlesSpent(int);

int main(void){
    
    printf("How long did you shower for, in minutes?: ");
    int minutesInShower = get_int();
    
    BottlesSpent(minutesInShower);
    
}

void BottlesSpent(minutesInShower){
    
    int bottles, ouncesOfWater;
    
    ouncesOfWater = minutesInShower * 192;
    
    bottles = ouncesOfWater / 16;
    
    printf("Bottles Wasted: %d\n", bottles);
    
}