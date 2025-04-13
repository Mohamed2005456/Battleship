#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants for board size and number of ships
#define GRID_SIZE 5
#define SHIP_COUNT 4

// Define a Ship with a name and a size
typedef struct {
    char *name;
    int size;
} Ship;

// Define the different types of ships (fleet)
Ship fleet[SHIP_COUNT] = {
    {"Battleship", 4},
    {"Cruiser", 3},
    {"Submarine", 2},
    {"Patrol Boat", 1}
};

// Game boards
char playerBoard[GRID_SIZE][GRID_SIZE];       // Players board which shows their ships
char aiBoard[GRID_SIZE][GRID_SIZE];           // AIs board ships are hidden
char aiTrackingBoard[GRID_SIZE][GRID_SIZE];   // Players view of AIs board (hits/misses)

// Function prototypes
void initializeBoards();

void printBoard(char board[GRID_SIZE][GRID_SIZE]);

void placeShipsRandomly(char board[GRID_SIZE][GRID_SIZE]);

void placeShipsManually(char board[GRID_SIZE][GRID_SIZE]);

void playerTurn();

void aiTurn();

int checkWin(char board[GRID_SIZE][GRID_SIZE]);

int main() {
    srand(time(NULL));  // Seed for random placement

    // Set up empty boards
    initializeBoards();

    // Let player place their ships
    printf("Place your ships:\n");
    printBoard(playerBoard);
    placeShipsManually(playerBoard);

    // AI places ships randomly
    placeShipsRandomly(aiBoard);

    // Game loop
    int gameOver = 0;
    while (!gameOver) {
        // Show player’s current board and tracks of AIs hits
        printf("\nPlayer's Board:\n");
        printBoard(playerBoard);

        printf("\nAI's Tracking Board:\n");
        printBoard(aiTrackingBoard);

        // Player attacks
        playerTurn();
        if (checkWin(aiBoard)) {
            printf("Congratulations! You win!\n");
            break;
        }

        // AI attacks
        aiTurn();
        if (checkWin(playerBoard)) {
            printf("AI wins! Better luck next time.\n");
            break;
        }
    }
    return 0;
}

// Fill all boards with water with the use of this sign (~)
void initializeBoards() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            playerBoard[i][j] = '~';
            aiBoard[i][j] = '~';
            aiTrackingBoard[i][j] = '~';
        }
    }
}

// Print a game board with labels
void printBoard(char board[GRID_SIZE][GRID_SIZE]) {
    printf("   A B C D E\n");
    
    for (int i = 0; i < GRID_SIZE; i++) {
        
        printf("%2d ", i + 1);
        for (int j = 0; j < GRID_SIZE; j++) {
            
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Randomly place all ships on a board and make sure no ships are colliding
void placeShipsRandomly(char board[GRID_SIZE][GRID_SIZE]) {
    for (int s = 0; s < SHIP_COUNT; s++) {
        int placed = 0;
        while (!placed) {
            
            int row = rand() % GRID_SIZE;
            
            int col = rand() % GRID_SIZE;
            
            int horizontal = rand() % 2;
            int fits = 1;

            // Horizontal placement of ship
            if (horizontal) {
                if (col + fleet[s].size > GRID_SIZE) continue;
                for (int i = 0; i < fleet[s].size; i++) {
                    if (board[row][col + i] != '~') {
                        fits = 0;
                        break;
                    }
                }
                if (fits) {
                    for (int i = 0; i < fleet[s].size; i++) {
                        board[row][col + i] = 'S';
                    }
                    placed = 1;
                }
            }
            // Vertical placement of ship
            else {
                if (row + fleet[s].size > GRID_SIZE) continue;
                for (int i = 0; i < fleet[s].size; i++) {
                    if (board[row + i][col] != '~') {
                        fits = 0;
                        break;
                    }
                }
                if (fits) {
                    for (int i = 0; i < fleet[s].size; i++) {
                        board[row + i][col] = 'S';
                    }
                    placed = 1;
                }
            }
        }
    }
}

// Lets player to place ships with coordinate and direction input
void placeShipsManually(char board[GRID_SIZE][GRID_SIZE]) {
    for (int s = 0; s < SHIP_COUNT; s++) {
        char colChar;
        int row, col, horizontal;
        int valid = 0;

        while (!valid) {
            // For placement
            printf("\nPlacing %s (%d units)\n", fleet[s].name, fleet[s].size);
            printf("Enter starting coordinate (e.g., A3): ");
            scanf(" %c%d", &colChar, &row);
            col = colChar - 'A';
            row--;

            printf("Enter direction (1 for horizontal, 0 for vertical): ");
            scanf("%d", &horizontal);

            // Validate input and position
            if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
                printf("Invalid starting coordinate. Try again.\n");
                continue;
            }

            int fits = 1;
            // Horizontal placement of the ship
            if (horizontal) {
                if (col + fleet[s].size > GRID_SIZE) {
                    printf("Ship does not fit horizontally. Try again.\n");
                    continue;
                }
                for (int i = 0; i < fleet[s].size; i++) {
                    if (board[row][col + i] != '~') {
                        fits = 0;
                        break;
                    }
                }
                if (fits) {
                    for (int i = 0; i < fleet[s].size; i++) {
                        board[row][col + i] = 'S';
                    }
                    valid = 1;
                } else {
                    printf("Space is occupied. Try again.\n");
                }
            }
            // Vertical placement of ship
            else {
                if (row + fleet[s].size > GRID_SIZE) {
                    printf("Ship does not fit vertically. Try again.\n");
                    continue;
                }
                for (int i = 0; i < fleet[s].size; i++) {
                    if (board[row + i][col] != '~') {
                        fits = 0;
                        break;
                    }
                }
                if (fits) {
                    for (int i = 0; i < fleet[s].size; i++) {
                        board[row + i][col] = 'S';
                    }
                    valid = 1;
                } else {
                    printf("Space is occupied. Try again.\n");
                }
            }

            // Show updated board
            if (!valid) {
                printf("Invalid placement. Try again.\n");
            }
            printBoard(board);
        }
    }
}

// Handle player's attack turn
void playerTurn() {
    int row, col;
    char colChar;
    int valid = 0;

    while (!valid) {
        printf("Enter attack coordinates (e.g., B5): ");
        scanf(" %c%d", &colChar, &row);
        col = colChar - 'A';
        row--;

        // Check bounds
        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
            printf("Coordinate does not exist. Try again.\n");
            continue;
        }

        // Check if already attacked
        if (aiTrackingBoard[row][col] == 'X' || aiTrackingBoard[row][col] == 'O') {
            printf("Coordinate has already been attacked. Try again.\n");
        } else {
            valid = 1;
            // Check for hit or miss
            if (aiBoard[row][col] != '~') {
                printf("Hit!\n");
                aiTrackingBoard[row][col] = 'X';
                aiBoard[row][col] = 'X';
            } else {
                printf("Miss!\n");
                aiTrackingBoard[row][col] = 'O';
            }
        }
    }
}

// Handle AI's attack turn
void aiTurn() {
    int row, col;

    // Randomly choose until unused coordinate is found
    do {
        row = rand() % GRID_SIZE;
        col = rand() % GRID_SIZE;
    } while (playerBoard[row][col] == 'X' || playerBoard[row][col] == 'O');

    // Apply hit or miss
    if (playerBoard[row][col] != '~') {
        printf("AI hit at %c%d!\n", 'A' + col, row + 1);
        playerBoard[row][col] = 'X';
    } else {
        printf("AI missed at %c%d.\n", 'A' + col, row + 1);
        playerBoard[row][col] = 'O';
    }
}

// Check if all ships on a board have been sunk
int checkWin(char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // Any remaining 'S' means ships still afloat
            if (board[i][j] != '~' && board[i][j] != 'X' && board[i][j] != 'O') {
                return 0;
            }
        }
    }
    return 1;  // All ships are destroyed
}
