# Board-Game-Ai-Bot

A simple turn-based board game for the terminal, featuring a 7x7 board and an AI opponent using the Minimax algorithm. Two players ("X" for human, "O" for AI) compete to control the board by moving their pieces and maximizing their mobility.

## Features

- 7x7 grid-based game board
- Human vs. AI gameplay
- Manual or random piece placement
- AI uses Minimax for move selection
- Game ends when neither player can move or max turns are reached
- Winner is determined by the number of available moves at the end

## Compiling and Running

This project requires a C compiler (such as gcc).

Compile with:
gcc main.c -o board-game-ai-bot

Run with:
./board-game-ai-bot

## Gameplay

1. Enter the number of pieces for each player and the maximum number of turns.
2. Choose whether to place pieces randomly or manually.
3. Players take turns moving their pieces to adjacent empty spaces (up, down, left, right).
4. The AI ("O") moves automatically using Minimax search.
5. The game ends when neither player can move or the maximum number of turns is reached.
6. The winner is the player with more movable spaces at the end.

## Example Input

Enter the number of pieces for Player 1 (X): 5
Enter the number of pieces for Player 2 (O): 5
Enter the maximum number of turns: 30
Place pieces randomly (R) or manually (M)? R

## Author

Mertkan İscan
