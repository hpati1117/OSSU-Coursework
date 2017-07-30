#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 26
#define CHAR_TO_NODE_INDEX(a) ((int)a - (int)'a')



//insert word into node
void insertNode(node* trieTree, char *word)
{
    // make a pointer to the tree we're working with and use it to navigate tree
    struct node *currentNode = trieTree;
    
    for (int i = 0; i < strlen(word); i++)
    {
        if (currentNode->children[word[CHAR_TO_NODE_INDEX(word[i])] == NULL)
        {
            currentNode->children[word[CHAR_TO_NODE_INDEX(word[i])] = malloc(sizeof(struct node));
            currentNode->children[word[CHAR_TO_NODE_INDEX(word[i])]->parent = currentNode;
        }
        
        currentNode = currentNode->children[word[CHAR_TO_NODE_INDEX(word[i])];
        
    }
    
    currentNode->occurrences += 1;
    
    printf("Node added\n");
    
}
//



int main(void)
{   // CODE
    
    printf("%i is maxsize, %i is node index given c\n", MAX_SIZE, CHAR_TO_NODE_INDEX('\''));
    
    // creates the first node
    typedef struct node
    {
        struct node *parent = NULL;
        struct node *children[MAX_SIZE];
        int occurences = 0;
        
    } node;

    
    struct node *root;
    
    char *firstword;
    scanf("Enter a word and hope for no errors!\n", firstword);
    
    insertNode(root, firstWord)
    
    
}

