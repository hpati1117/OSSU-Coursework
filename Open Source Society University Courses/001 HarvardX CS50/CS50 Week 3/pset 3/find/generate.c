/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // TODO: comment me
    // Check if the user entered anything other than 2 or 3 args. If so,
    // return 1.
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // TODO: comment me
    // Convert the 'n' number the user entered from a string to an int.
    int n = atoi(argv[1]);

    // TODO: comment me
    // If there are 3 args...
    if (argc == 3)
    {
        // Run srand with argv[2] as the seed.
        srand48((long) atoi(argv[2]));
    }
    else
    {
        //Otherwise, run it without a seed.
        srand48((long) time(NULL));
    }

    // TODO: comment me
    // For X number of times, print out a random number between 0 and LIMIT.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
