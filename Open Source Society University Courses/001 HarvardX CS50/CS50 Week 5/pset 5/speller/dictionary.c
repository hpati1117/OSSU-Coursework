/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "dictionary.h"

/*
~~NOTES~~(Can ignore)

What I learn this time was that, if something needs to be used by multiple functions,
dont put that thing inside a function. I'm looking at you trie, rootNode, and 
totalWordsLoaded! I was stuck on this for over a week before I realized this simple
thing. Oh well, we all gotta learn somehow right? :/
*/



// trie Structure
typedef struct trie{
    
    bool isWord;
    struct trie *children[27];
    
}
trie;

// first node of the trie
trie *rootNode;



// int to keep track of total number of words loaded
int totalWordsLoaded = 0;



// function to free memory taken up by trie
void freeTrieNode(trie* currentNode)
{
    // for each item in the node 
    for (int i = 0; i < 27; i++)
    {
        
        // if the node being itterated through is not null
        if (currentNode -> children[i] != NULL)
        {
            
            // cast this same function using that node
            freeTrieNode(currentNode -> children[i]);
            
        }
    }
    
    // free the memory used by that node and return
    free(currentNode);
    return;
    
}



/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    
    // create an int variable to store the length of word
    int wordLength = strlen(word);
    
    // create a new variable to hold the lowercased word
    char lowerWord[wordLength + 1];
    
    // itterate through each char and, if it's capitalized, lower case it
    // by doing this, we can assure that the word will always be lowercase
    for (int i = 0; i < wordLength; i++)
    {
        // if the character in word is uppercase, lowercase it and
        // add it to lowerWord
        if (isupper(word[i]))
        {
            
            lowerWord[i] = tolower(word[i]);
            
        }
        
        // otherwise, just add it to lowerWord
        else
        {
            
            lowerWord[i] = word[i];
            
        }
        
    }
    
    // create a currentNode that will start at the root node
    trie* currentNode = rootNode;
    
    // itterate through each character in lowerWord and...
    for (int j = 0; j < wordLength; j++)
    {
        
        // if the current character is an apostrophe and it exists in the
        // node, make the currentNode become it
        if (lowerWord[j] == '\'' && 
            currentNode -> children[26] != NULL)
        {
            
            currentNode = currentNode -> children[26];
            
        }
        
        // else if the current letter's index exists in the node, make
        // currentNode become it
        else if (currentNode -> children[lowerWord[j] - (int)'a'] != NULL)
        {
            
            currentNode = currentNode -> children[lowerWord[j] - (int)'a'];
            
        }
        
        // otherwise, it doesn't exists in the node so it's not a word
        else
        {
            
            return false;
            
        }
        
    }
    
    // return the value of isWord at the final character. If what we
    // followed was a word, it will return true. Otherwise, false.
    return currentNode -> isWord;
}



//DO THIS BELOW FIRST

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
 
bool load(const char *dictionary)
{
    // open the dictionary passed in and ensure it exists
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "File opening caused error\n");
        return false;
    }
    
    // char of size 46 for storing work being itterated through
    char word[LENGTH + 1];
    
    // int to store the location of a char in the trie
    int charIndex;
    
    // allocates memory for the root node
    rootNode = (trie*)malloc(sizeof(trie));
    
    // set current node to null
    trie *currentNode = NULL;
    
    
    // while the word returned is not the end of the file
    while(fscanf(inptr, "%s", word) != EOF)
    {
        // set currentNode to be the root node
        currentNode = rootNode;
        
        // set wordLength to be the length of the word being itterated
        // through and itterate through each char in word
        for(int wordIndex = 0, wordLength = strlen(word); 
            wordIndex < wordLength; wordIndex++)
        {
            
            // if the character happens to be a terminating character,
            // break out of the for loop because we hit the end of the
            // word somehow
            if (word[wordIndex] == '\0')
            {
                
                break;
                
            }
            
            // if the current char is an apostrophe, set the
            // index it will be placed in to the last index (26)
            if (word[wordIndex] == '\'')
            {
                
                charIndex = 26;
                
            }
            
            // otherwise, set the charIndex to be the char minus 'a'
            else
            {
                
                charIndex = (int)word[wordIndex] - (int)'a';
                
            }
            
            
            
            // if the char's index is not made yet, allocate memory for it
            if (currentNode -> children[charIndex] == NULL)
            {
                
                currentNode -> children[charIndex] = (trie*)malloc(sizeof(trie));
                currentNode = currentNode -> children[charIndex];
                
            }
            
            // otherwise if it's already made, make currentNode point to
            // it
            else
            {
                
                currentNode = currentNode -> children[charIndex];
                
            }
        }
        
        // once itterated through a word, increase the word count by 1
        // and set the bool isWord inside the node of the trie to true
        currentNode->isWord = true;
        totalWordsLoaded++;
        
    }
    
    // close the file and return true for successfully completing
    fclose(inptr);
    return true;
}






/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    return totalWordsLoaded;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // creates a pointer to the root node
    trie* currentNode = rootNode;
    
    // calls freeTrieNode to free up the memory taken by the trie
    freeTrieNode(currentNode);
    
    // return true upon completion
    return true;
}
