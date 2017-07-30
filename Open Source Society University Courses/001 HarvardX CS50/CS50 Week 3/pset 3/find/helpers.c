/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Searches for a value using binary search
 */
bool binarySearch(int value, int values[], int n)
{

    //Creates variables for the start (0), end (n-1), and middle
    //(start + end / 2) indexes of the array.
    int start = 0;
    int end = n - 1;
    int middle = (start + end) / 2;

    //While the end is greater than or equal to the start, check...
    while (end >= start)
        {

        //If the value in the array is the one we're looking for. If so,
        //return true. Otherwise...
        if (values[middle] == value)
        {
            return true;
        }
        
        //If the number we're looking for is smaller, set the end of the array
        //to be the middle of the array minus 1. Otherwise...
        else if (values[middle] > value)
        {
            end = middle - 1;
        }
        
        //That means the number we're looking for is bigger, so we set
        //the start to be in the middle plus 1.
        else
        {
            start = middle + 1;
        }
        
        //Afterwards, set the middle to be the sum of the new start and
        //end divided by 2.
        middle = (start + end) / 2;

    }

    return false;

}

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    // n is the size of the array, array is sorted.
    
    //As long as the value we're looking for is bigger than 0, search
    //for the number using binary search.
    if (!(value < 0))
    {
        
        return binarySearch(value, values, n);
        
    }
    
    return false;
}

/**
 * Sorts array of n values using bubble sort.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    // n is size of array

    //Create a bool to check whether to continue running the do while
    //function below.
    bool cont;
    
    //Sorts the array using bubble sort.
    do
    {
        
        cont = false;
            
        for (int j = 0; j < n - 1; j++)
        {

            if (values[j] > values[j + 1])
            {
            
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
                cont = true;
            }
        
        }
    
    }while (cont);
    
    return;
    
}

