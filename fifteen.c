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
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

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
    init();

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
        if (won())
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
void init(void)
{
    // initialize tile to last number based on board dimensions
    int tile = d*d-1;
    // iterate over each row
    for (int row = 0; row < d; row++)
    {
        // iterate over each column
        for (int column = 0; column < d; column++)
        {
            // if board dimensions are even, swap the starting positions of 1 and 2
            if ((d % 2 == 0) && (tile == 2))
            {
                board[row][column] = 1;
                board[row][column+1] = 2;
                break;
            }
            // else, fill in tiles in decreasing order
            else
            {
            board[row][column] = tile;
            tile--;
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // iterate over each row
    for (int row = 0; row < d; row++)
    {
        // iterate over each column
        for (int column = 0; column < d; column++)
        {
            // print padded "_" in place of 0
            if (board[row][column] == 0)
            {
                printf("%*s", 3, "_");
            }
            // print all other numbers, padded 3 spaces
            else
            {
            printf("%*d", 3, board[row][column]);
            }
        }
        // print each row on a separate line
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int zero_row;
    int zero_column;
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            // find the current position of the empty space
            if (board[row][column] == 0)
            {
                zero_row = row;
                zero_column = column;
            }
        }
    }
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            // find the current position of the tile to move
            if (board[row][column] == tile)
            {
                // calculate the Manhattan distance between the chosen tile and the empty space
                int position = abs(row - zero_row) + abs(column - zero_column);
                // move is valid if the Manhattan distance is equal to 1
                if (position == 1)
                {
                    // swap the positions of the tile and the empty space
                    board[row][column] = 0;
                    board[zero_row][zero_column] = tile;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int counter = 1;
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            if (board[row][column] == counter)
            {
                counter++;
            }
            else if (counter == d*d)
            {
                return true;
            }
            else
            {
                break;
            }
        }
    }
    return false;
}
