#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define BOARD_SIZE 7

int bestScore = INT_MIN;
int worstScore = INT_MAX;


void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool isAdjacent(int x, int y, int newX, int newY);
bool isPlayersPiece(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char player);
bool isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int newX, int newY);
void makeMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int newX, int newY, char player);
void undoMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int prevX, int prevY, char player);
void placeRandomPieces(char board[BOARD_SIZE][BOARD_SIZE], int playerPieces, char player);
void placePiecesManually(char board[BOARD_SIZE][BOARD_SIZE], int playerPieces, char player);
void printMoveOptions(char board[BOARD_SIZE][BOARD_SIZE], char player);
int getMobilityScore(char board[BOARD_SIZE][BOARD_SIZE], int x, int y);
int evaluateBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool canMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y);
bool gameOver(char board[BOARD_SIZE][BOARD_SIZE], int currentTurn, int maxTurns);
int tryAllMoves(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizingPlayer, int currentTurn, int maxTurns, int bestMove[4]);
int minmax(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizingPlayer, int currentTurn, int maxTurns);
char determineWinnerBasedOnMovableSpaces(char board[BOARD_SIZE][BOARD_SIZE]);
bool hasMovablePieces(char board[BOARD_SIZE][BOARD_SIZE], char player);

int main() {

    int depth = 3;
    char winner = 'T'; // Default to Tie
    char board[BOARD_SIZE][BOARD_SIZE];
    int player1Pieces, player2Pieces, maxTurns, currentTurn = 0;
    char currentPlayer = 'X';
    char placementMethod;

    //initial board settings
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '.';
        }
    }

    // take player piece count and max turn number
    printf("Enter the number of pieces for Player 1 (X): ");
    scanf("%d", &player1Pieces);
    printf("Enter the number of pieces for Player 2 (O): ");
    scanf("%d", &player2Pieces);
    printf("Enter the maximum number of turns: ");
    scanf("%d", &maxTurns);

    // ask for placing random or manual
    printf("Place pieces randomly (R) or manually (M)? ");
    scanf(" %c", &placementMethod);

    if (placementMethod == 'R' || placementMethod == 'r') {

        //random piece inserting
        placeRandomPieces(board, player1Pieces, 'X');
        placeRandomPieces(board, player2Pieces, 'O');

    } else if (placementMethod == 'M' || placementMethod == 'm') {

        //manual piece interting
        placePiecesManually(board, player1Pieces, 'X');
        placePiecesManually(board, player2Pieces, 'O');
    }

    // Game Loop
    while (currentTurn < maxTurns && !gameOver(board, currentTurn, maxTurns)) {

        printBoard(board);

        if (currentPlayer == 'X') {
            // Human player move
            printMoveOptions(board, currentPlayer);
            int x, y, newX, newY;
            char sourceCol, destCol;

            // Check if the current player has movable pieces
            if (!hasMovablePieces(board, currentPlayer)) {
            // The current player has no moves left, so the other player wins
            winner = (currentPlayer == 'X') ? 'O' : 'X';
            printf("Player %c has no moves left. Player %c wins!\n", currentPlayer, winner);
            return 0;
            }

            printf("Player %c, enter piece to move (e.g., d4): ", currentPlayer);
            scanf(" %c%d", &sourceCol, &y);
            x = sourceCol - 'a';
            y--;

            // check piece and move is valid
            while (!isPlayersPiece(board, x, y, currentPlayer)) {
                printf("Invalid piece. Try again.\n");
                printf("Player %c, enter piece to move (e.g., d4): ", currentPlayer);
                scanf(" %c%d", &sourceCol, &y);
                x = sourceCol - 'a';
                y--;
            }

            bool validMove = false;
            while (!validMove) {
                printf("Enter new position for %c%d (e.g., e4): ", sourceCol, y + 1);
                scanf(" %c%d", &destCol, &newY);
                newX = destCol - 'a';
                newY--;

                if (isValidMove(board, x, y, newX, newY)) {
                    validMove = true;
                } else {
                    printf("Invalid move. Try again.\n");
                }
            }

            makeMove(board, x, y, newX, newY, currentPlayer);
        } else {
            int bestMoveScore = INT_MIN;
            int bestMove[4]; // store x, y, newX, newY for best move

            bestMoveScore = tryAllMoves(board, depth, true, currentTurn, maxTurns, bestMove);

            if (bestMoveScore != INT_MIN) {
                // make best move
                makeMove(board, bestMove[0], bestMove[1], bestMove[2], bestMove[3], 'O');
                printf("AI moved from %c%d to %c%d\n", 'a' + bestMove[0], bestMove[1] + 1, 'a' + bestMove[2], bestMove[3] + 1);
            } else {
                printf("No valid moves for AI.\n");
            }
        }

        currentTurn++;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    printBoard(board);
    winner = determineWinnerBasedOnMovableSpaces(board);
    if (winner == 'T') {
        printf("The game is a tie.\n");
    } else {
        printf("Player %c wins the game.\n", winner);
    }

    // game ending message
    printf("Game over after %d turns.\n", currentTurn);

    return 0;
}


int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    
    printf("  1 2 3 4 5 6 7\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c", 'a' + i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}

bool isAdjacent(int x, int y, int newX, int newY) {
    
    return (x == newX && abs(y - newY) == 1) || (y == newY && abs(x - newX) == 1);
}

bool isPlayersPiece(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char player) {
    return board[x][y] == player;
}

bool isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int newX, int newY) {
    if (newX < 0 || newX >= BOARD_SIZE || newY < 0 || newY >= BOARD_SIZE) return false;
    if (!isAdjacent(x, y, newX, newY)) return false; 
    return board[newX][newY] == '.';
}

void makeMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int newX, int newY, char player) {
    board[x][y] = '.';
    board[newX][newY] = player;
}

void undoMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int prevX, int prevY, char player) {

    board[prevX][prevY] = player;


    board[x][y] = '.';
}

void placeRandomPieces(char board[BOARD_SIZE][BOARD_SIZE], int playerPieces, char player) {
    srand(time(NULL)); 

    while (playerPieces > 0) {
        int x = rand() % BOARD_SIZE;
        int y = rand() % BOARD_SIZE;

        if (board[x][y] == '.') {
            board[x][y] = player;
            playerPieces--;
        }
    }
}

void placePiecesManually(char board[BOARD_SIZE][BOARD_SIZE], int playerPieces, char player) {
    int x, y;
    char col;
    for (int i = 0; i < playerPieces; i++) {
        printf("Player %c, enter position for piece %d (e.g., d4): ", player, i + 1);
        scanf(" %c%d", &col, &y);
        x = col - 'a';
        y--;

        while (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || board[x][y] != '.') {
            printf("Invalid position. Try again.\n");
            printf("Player %c, enter position for piece %d (e.g., d4): ", player, i + 1);
            scanf(" %c%d", &col, &y);
            x = col - 'a';
            y--;
        }

        board[x][y] = player;
    }
}

void printMoveOptions(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    printf("Player %c can move the following pieces:\n", player);
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (board[x][y] == player) {
                printf("%c%d: ", 'a' + x, y + 1);

                if (y > 0 && board[x][y - 1] == '.') printf("%c%d ", 'a' + x, y);     // Sol
                if (y < BOARD_SIZE - 1 && board[x][y + 1] == '.') printf("%c%d ", 'a' + x, y + 2); // Sağ
                if (x > 0 && board[x - 1][y] == '.') printf("%c%d ", 'a' + x - 1, y + 1); // Yukarı
                if (x < BOARD_SIZE - 1 && board[x + 1][y] == '.') printf("%c%d ", 'a' + x + 1, y + 1); // Aşağı
                printf("\n");
            }
        }
    }
}

int getMobilityScore(char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    int mobilityScore = 0;
    
    // left
    if (y > 0 && board[x][y - 1] == '.') mobilityScore++;
    // right
    if (y < BOARD_SIZE - 1 && board[x][y + 1] == '.') mobilityScore++;
    // up
    if (x > 0 && board[x - 1][y] == '.') mobilityScore++;
    // down
    if (x < BOARD_SIZE - 1 && board[x + 1][y] == '.') mobilityScore++;

    return mobilityScore;
}

int evaluateBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    int aiScore = 0;
    int playerScore = 0;

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (board[x][y] == 'O') { 
                aiScore += getMobilityScore(board, x, y);
            } else if (board[x][y] == 'X') { 
                playerScore += getMobilityScore(board, x, y);
            }
        }
    }

   
    return aiScore - playerScore;
}

bool canMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {

    return (x > 0 && board[x - 1][y] == '.') ||
           (x < BOARD_SIZE - 1 && board[x + 1][y] == '.') ||
           (y > 0 && board[x][y - 1] == '.') ||
           (y < BOARD_SIZE - 1 && board[x][y + 1] == '.');
}

bool gameOver(char board[BOARD_SIZE][BOARD_SIZE], int currentTurn, int maxTurns) {

    if (currentTurn >= maxTurns) {
        return true;
    }

    // If any player has no more pieces that can move, the game ends.
    bool canPlayerMove = false;
    bool canAIMove = false;

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (board[x][y] == 'X' && canMove(board, x, y)) {
                canPlayerMove = true;
            }
            if (board[x][y] == 'O' && canMove(board, x, y)) {
                canAIMove = true;
            }
        }
    }

    // if both players can make moves game still goes on
    return !(canPlayerMove && canAIMove);
}

int tryAllMoves(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizingPlayer, int currentTurn, int maxTurns, int bestMove[4]) {
    int localBestScore = (isMaximizingPlayer) ? INT_MIN : INT_MAX;

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            char player = (isMaximizingPlayer) ? 'O' : 'X';
            if (board[x][y] == player) {
                // try up down left right moves
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int newX = x + dx;
                        int newY = y + dy;
                        if (isValidMove(board, x, y, newX, newY)) {
                            makeMove(board, x, y, newX, newY, player);

                            int score = minmax(board, depth - 1, !isMaximizingPlayer, currentTurn + 1, maxTurns);
                            if ((isMaximizingPlayer && score > localBestScore) || (!isMaximizingPlayer && score < localBestScore)) {
                                localBestScore = score;
                                bestMove[0] = x;
                                bestMove[1] = y;
                                bestMove[2] = newX;
                                bestMove[3] = newY;
                            }
                            undoMove(board, newX, newY, x, y, player);
                        }
                    }
                }
            }
        }
    }
    return localBestScore;
}



int minmax(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizingPlayer, int currentTurn, int maxTurns) {
    int score = evaluateBoard(board);

    // Check terminal conditions: Win, Lose, Draw, or Depth Limit
    if (gameOver(board, currentTurn, maxTurns) || depth == 0) {
        return score;
    }

    if (isMaximizingPlayer) {

        int dummyBestMove[4];
        bestScore = tryAllMoves(board, depth, true, currentTurn, maxTurns, dummyBestMove);

        return bestScore;
    } else {

        int dummyBestMove[4];
        worstScore = tryAllMoves(board, depth, false, currentTurn, maxTurns, dummyBestMove);

        return worstScore;
    }
}

char determineWinnerBasedOnMovableSpaces(char board[BOARD_SIZE][BOARD_SIZE]) {
    int movableSpacesX = 0;
    int movableSpacesO = 0;

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (board[x][y] == 'X') {
                movableSpacesX += getMobilityScore(board, x, y);
            } else if (board[x][y] == 'O') {
                movableSpacesO += getMobilityScore(board, x, y);
            }
        }
    }

    // Print the movable place count for each player
    printf("Movable spaces - Player X: %d, Player O: %d\n", movableSpacesX, movableSpacesO);

    if (movableSpacesX > movableSpacesO) {
        return 'X';
    } else if (movableSpacesO > movableSpacesX) {
        return 'O';
    } else {
        return 'T'; // Tie
    }
}

bool hasMovablePieces(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (isPlayersPiece(board, x, y, player) && canMove(board, x, y)) {
                // If the player has at least one piece that can move, return true
                return true;
            }
        }
    }
    // If no movable pieces are found, return false
    return false;
}