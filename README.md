# Board-Game-Ai-Bot
This program is an implementation of a two-player board game where the objective is to outmaneuver the opponent by strategically moving pieces on a 7x7 grid. The game utilizes an AI opponent that employs the Minimax algorithm to make decisions.

Key Features:

Game Rules: Players take turns making moves on the board, and the game continues until a specified maximum number of turns is reached or one player has no more movable pieces.
Player Interaction: Player 1 ('X') makes manual moves by selecting a piece and choosing a valid destination. Player 2 ('O') is controlled by an AI opponent that utilizes the Minimax algorithm to make strategic decisions.
Piece Placement: Players can either place their pieces randomly or manually on the board at the beginning of the game.
Move Validation: The program ensures that moves are valid, including checking for adjacent positions and empty destination spots.
Game Evaluation: The AI evaluates the board based on the mobility of each player's pieces, aiming to maximize its own score while minimizing the opponent's.
