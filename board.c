#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Board {
  int squares[8][8];
};
struct Board game_board;

struct Player {
  int score;
  char name[20];
};
struct Player black;
struct Player white;

int player_turn = 1;

int move_exists();

// This function outputs the board
void output_board() {
  printf("\n");

  char pieces[3] = {'-', 'B', 'W'}; // Possible pieces

  for(int y=0; y<8; ++y) {
    printf("%d ", y+1);
    for(int x=0; x<8; ++x) {
      printf("%c ", pieces[game_board.squares[y][x]]);
    }
    printf("\n");
  }

  printf("  ");
  for(int i=0; i<8; ++i) {
    printf("%c ", 'a'+i);
  }
  printf("\n\n");
}

int make_move(char move[2], int valid_check) {
  int valid_move = 0;

  if(move[0] == 'p') { // Check if player decided to pass
    if(move_exists()) { // Don't allow player to pass unless there's no possible moves
      printf("You can't pass unless you have no valid moves.\n");
    } else {
      valid_move = 1;
    }
  } else {
    // Get move x and y
    int x = move[0]-'a';
    int y = move[1]-'1';

    if(game_board.squares[y][x] == 0) { // Ensure place player wants to move to is empty
      // Here we check every possible diagnal/horizontal/vertical line for possible valid moves
      for(int y_inc=-1; y_inc<=1; ++y_inc) {
        for(int x_inc=-1; x_inc<=1; ++x_inc) {
          if(y_inc != 0 || x_inc != 0) {
            int temp_y = y+y_inc;
            int temp_x = x+x_inc;

            int possible_line = 0;
            int line_length = 0;

            if(temp_y < 8 && temp_x < 8) { // Ensure we don't check off board
              if(game_board.squares[temp_y][temp_x] != player_turn) { // Continue along line so long as the current piece isn't the current players
                while(game_board.squares[temp_y][temp_x] != 0) { // Break if you reach an empty spot
                  // Increment temp coords
                  temp_y += y_inc;
                  temp_x += x_inc;
                  ++line_length;
                  if(game_board.squares[temp_y][temp_x] == player_turn) { // If we come across a piece of our own we finish the line
                    possible_line = 1;
                    break;
                  }
                }
              }
            }

            if(possible_line) { // If a line can be made then
              valid_move = 1;   // We know move is valid
              if(!valid_check) { // Checks to make sure we actually want to change board
                for(int i=1; i<=line_length; ++i) {
                  game_board.squares[y+(y_inc*i)][x+(x_inc*i)] = player_turn; // Change piece
                  // Change scores
                  if(player_turn == 1) {
                    ++black.score;
                    --white.score;
                  } else {
                    ++white.score;
                    --black.score;
                  }
                }
              }
            }
          }
        }
      }
    }
    if(valid_move) {
      // Change chosen piece and squares based on turn
      game_board.squares[y][x] = player_turn;
      if(player_turn == 1) {
        ++black.score;
      } else {
        ++white.score;
      }
    }

  }

  return valid_move;
}

// Loop through every possible move and see if there exists a possible one
int move_exists() {
  char move[2];
  int has_move = 0;
  for(int y=0; y<8; ++y) {
    for(int x=0; x<8; ++x) {
      move[0] = 'a'+x;
      move[1] = '1'+y;
      if(make_move(move, 1)) {
        has_move = 1;
        break;
      }
    }
  }
  return has_move;
}

// This code appends winner to file
void write_winner_to_file() {
  FILE *fp;
  fp = fopen("othello-results.txt", "a");

  // Get time
  time_t t = time(NULL);
  struct tm now = *localtime(&t);
  fprintf(fp, "%d-%02d-%02d %02d:%02d\n", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min);
  // Output who won based on score
  if(black.score == white.score) {
    fprintf(fp, "Score: %s (Black) %d:%d %s (White)\n", black.name, black.score, white.score, white.name);
    fprintf(fp, "Game was a draw\n");
  } else if(black.score > white.score) {
    fprintf(fp, "Score: %s (Black) %d:%d %s (White)\n", black.name, black.score, white.score, white.name);
    fprintf(fp, "%s (Black) won\n", black.name);
  } else {
    fprintf(fp, "Score: %s (Black) %d:%d %s (White)\n", black.name, black.score, white.score, white.name);
    fprintf(fp, "s%s (White) won\n", white.name);
  }

  fclose(fp);
}
