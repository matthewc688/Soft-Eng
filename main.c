#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.c"


int main() {
  // Welcome player
  printf("     *** Welcome to Othello! ***     \n\n");

  printf("Enter name of Player 1 (Black): ");
  scanf("%s", black.name);
  printf("Enter name of Player 2 (White): ");
  scanf("%s", white.name);

  // Initialize player score
  black.score = 2;
  white.score = 2;

  // Fill board with empty squares
  for(int y=0; y<8; ++y) {
    for(int x=0; x<8; ++x) {
      game_board.squares[y][x] = 0;
    }
  }
  // Set up initial game pieces
  game_board.squares[3][4] = 1;
  game_board.squares[4][3] = 1;
  game_board.squares[3][3] = 2;
  game_board.squares[4][4] = 2;

  int pass_count = 0; // If pass count is ever 2 we now both players passed in a row and therefore the game is over
  while(1) {
    output_board();

    char move[2];
    printf("Score: %s (Black) %d:%d %s (White)\n", black.name, black.score, white.score, white.name); // Output scores

    // Get current player name
    char *current_player_name;
    if(player_turn == 1) {
      current_player_name = black.name;
    } else {
      current_player_name = white.name;
    }

    // Ask for move
    printf("%s, make your move : ", current_player_name);
    scanf("%s", move);

    // Keep asking for move until valid move is provided
    while(!(make_move(move, 0))) {
      printf("That was not a valid move\n");
      printf("%s, make your move : ", current_player_name);
      scanf("%s", move);
    }

    // Increment pass counter if player passed
    if(move[0] == 'p') {
      pass_count += 1;
    } else {
      pass_count = 0;
    }

    if(black.score + white.score == 64) { // End game if board is full
      break;
    }
    if(pass_count >= 2) { // End game if neither players have a valid move available
      break;
    }

    // Swap moves
    if(player_turn == 1) {
      player_turn = 2;
    } else {
      player_turn = 1;
    }
  }

  // Game end sequence
  printf("\nGame over\n");
  output_board();

  if(black.score == white.score) {
    printf("Score: %s (Black) %d:%d %s (White)\n", black.name, black.score, white.score, white.name);
    printf("Game was a draw.");
  } else if(black.score > white.score) {
    printf("Score: %s (Black) %d:%d %s (White)\n", black.name, black.score, white.score, white.name);
    printf("%s wins!", black.name);
  } else {
    printf("Score: %s (Black) %d:%d %s (White)\n", black.name, black.score, white.score, white.name);
    printf("%s wins!", white.name);
  }

  write_winner_to_file(); 
}
