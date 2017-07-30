/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(int d);
void draw(void);
bool move(int tile);
bool won(int d);
void moveTile(int zeroPosX, int zeroPosY, int tilePosX, int tilePosY);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init(d);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int d)
{
    // TODO
    
    //Creates the board 2D array and a variable that will be each number
    //to be added to the array (d*d - 1).
    int counter = (d * d) - 1;
    //int board[d][d];
    
    //For each element in the 2D array...
    for (int i = 0; i < d; i++){
        
        for (int j = 0; j < d; j++){
            
            //assign the current element to be counter, then subract 1
            //from counter.
            board[i][j] = counter;
            counter--;
            
        }
        
    }
    
    //Swaps 2 & 1 if the number of elements in the array is odd.
    if (board[0][0] % 2 != 0){
        
        //stores 2
        int temp = board[d - 1][d - 3];
        
        //swaps 2 with 1
        board[d - 1][d - 3] = board[d - 1][d - 2];
        board[d - 1][d - 2] = temp;
        
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    
    //Prints out each element in the 2D array sans 0, where it instead
    //prints out ' _'. ALso, each integer will have 3 spaces to use.
    for (int i = 0; i < d; i++){
        
        for (int j = 0; j < d; j++){
            
            if (board[i][j] == 0){
                
                printf("  _");
                
            }else{
                
                printf("%3i", board[i][j]);
                
            }
        }
        
        printf("\n");
        
    }
    
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    //tile is the number on the board the user wants to switch to.
    
    //Creates variables to store the location of the underscore/0 and
    //of the tile the user wants to switch to.
    int zeroPosX, zeroPosY, tilePosX, tilePosY;
    
    //Finds the cordinates for the tile and underscore/0 and assigns
    //them to their appropriate variables.
    for (int i = 0; i < d; i++){
        
        for (int j = 0; j < d; j++){
            
            if (board[i][j] == 0){
                
                zeroPosX = i;
                zeroPosY = j;
                
            }
            
            if (board[i][j] == tile){
                
                tilePosX = i;
                tilePosY = j;
                
            }
            
        }
    
    }
    
    //Checks if the tile the user wants to move to is above, below,
    //left, or right of the underscore/0 tile. If not, returns false
    //for it being an illegal move. Otherwise, calls 'moveTile' to
    //switch the tiles and returns true.
    if (zeroPosX + 1 == tilePosX && zeroPosY == tilePosY){
        moveTile(zeroPosX, zeroPosY, tilePosX, tilePosY);
        return true;
    }
    
    else if (zeroPosX - 1 == tilePosX && zeroPosY == tilePosY){
        moveTile(zeroPosX, zeroPosY, tilePosX, tilePosY);
        return true;
    }
    
    else if (zeroPosY + 1 == tilePosY && zeroPosX == tilePosX){
        moveTile(zeroPosX, zeroPosY, tilePosX, tilePosY);
        return true;
    }
    
    else if (zeroPosY - 1 == tilePosY && zeroPosX == tilePosX){
        moveTile(zeroPosX, zeroPosY, tilePosX, tilePosY);
        return true;
    }
    else{
        return false;
    }

}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int d)
{
    // TODO
    //Creates a variable for the number that should be at the end and
    //the number that should be at the start (1) called maxNumber and 
    //current, respectively.
    int maxNumber = (d * d) - 1;
    int current = 1;
    
    //Itterates through the board and checks if the current number is
    //equal to the number being itterated through. If it is, the
    //loop continues. If it's not, it checks if the current number
    //is 0 and the last number was the hoghest number on the board.
    //If it is, it returns true. Otherwise, it means the board isn't
    //in order and it returns false.
    for (int i = 0; i < d; i++){
        
        for (int j = 0; j < d; j++){
            
            if (!(board[i][j] == current)){
                
                if (board[i][j] == 0 && board[i][j - 1] == maxNumber){
                    return true;
                }
                
                else{
                    return false;
                }
                
            }
            
            //Increases the current number to be checked for the next
            //loop.
            current++;
            
        }
        
    }
    
    return false;
    
}

/**
 * Moves the tiles around based on the positions entered.
 */
void moveTile(int zeroPosX, int zeroPosY, int tilePosX, int tilePosY)
{
    
    //Swaps 2 tiles around.
    board[zeroPosX][zeroPosY] = board[tilePosX][tilePosY];
    board[tilePosX][tilePosY] = 0;
    
}
