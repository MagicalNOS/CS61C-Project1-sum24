#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"
#include <sys/types.h>

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  
  game_state_t* start = (game_state_t*)malloc(sizeof(game_state_t));
  start->num_rows = 18;
  start->num_snakes = 1;

  // Allocate memory for the board
  start->board = (char**)malloc(sizeof(char*) * start->num_rows);
  for (int i = 0; i < start->num_rows; i++) {
    start->board[i] = (char*)malloc(sizeof(char) * 21); // 20 chars + null terminator
    strcpy(start->board[i],"#                  #");
  }

  strcpy(start->board[0],"####################");
  strcpy(start->board[17],"####################");

  // Mistakes
  // start->board[0] = "####################";
  // start->board[17] = "####################";


  //init fruit
  strcpy(start->board[2], "# d>D    *         #");
  
  //init the snake
  snake_t* s1 = (snake_t*)malloc(sizeof(snake_t));
  s1->live = true;
  s1->tail_row = 2; s1->tail_col = 2;
  s1->head_row = 2; s1->head_col = 4;
  start->snakes = s1;  

  return start;
}

/* Task 2 */
void free_state(game_state_t *state) {
  // TODO: Implement this function.

  free(state->snakes);

  for(int i = 0; i < state->num_rows; i++){
    free(state->board[i]);
  }

  free(state->board);

  free(state);

  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {

  // TODO: Implement this function.
  for(int i = 0; i < state->num_rows; i++){
    fprintf(fp,"%s\n",state->board[i]);
  }

  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  char* helper_str = "wasd";
  for(int i = 0; i < 4; i++){
    if(c == helper_str[i]){
      return true;
    }
  }

  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  char* helper_str = "WASDx";
  for(int i = 0; i < 5; i++){
    if(c == helper_str[i]){
      return true;
    }
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  char* helper_str = "wasd^<v>WASDx";
  for(int i = 0; i < 13; i++){
    if(c == helper_str[i]){
      return true;
    }
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  char* helper_body = "^<v>";
  char* helper_tail = "wasd";
  
  for(int i = 0; i < 4; i++){
    if(c == helper_body[i]){
      c = helper_tail[i];
    }
  }
  return c;
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  char* helper_body = "^<v>";
  char* helper_head = "WASD";
  
  for(int i = 0; i < 4; i++){
    if(c == helper_head[i]){
      c = helper_body[i];
    }
  }
  return c;
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if(c == 'v' || c == 's' || c == 'S'){
    cur_row++;
  }
  else if(c == '^' || c == 'w' || c == 'W'){
    cur_row--;
  }

  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if(c == '>' || c == 'd' || c == 'D'){
    cur_col++;
  }
  else if(c == '<' || c == 'a' || c == 'A'){
    cur_col--;
  }

  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.

  char head_forward = get_board_at(state,((state->snakes) + snum)->head_row,((state->snakes) + snum)->head_col);

  unsigned int next_col = get_next_col(((state->snakes) + snum)->head_col,head_forward);
  unsigned int next_row = get_next_row(((state->snakes) + snum)->head_row,head_forward);

  return get_board_at(state,next_row,next_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.

  char head_forward = get_board_at(state,((state->snakes) + snum)->head_row,((state->snakes) + snum)->head_col);

  char body_set = head_to_body(head_forward);

  set_board_at(state,((state->snakes) + snum)->head_row,((state->snakes) + snum)->head_col,body_set);

  unsigned int next_col = get_next_col(((state->snakes) + snum)->head_col,head_forward);
  unsigned int next_row = get_next_row(((state->snakes) + snum)->head_row,head_forward);

  (state->snakes + snum)->head_col = next_col;
  (state->snakes + snum)->head_row = next_row;

  set_board_at(state,((state->snakes) + snum)->head_row,((state->snakes) + snum)->head_col,head_forward);

  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  char tail_forward = get_board_at(state,((state->snakes) + snum)->tail_row,((state->snakes) + snum)->tail_col);

  unsigned int next_col = get_next_col(((state->snakes) + snum)->tail_col,tail_forward);
  unsigned int next_row = get_next_row(((state->snakes) + snum)->tail_row,tail_forward);

  char tail_set = body_to_tail(get_board_at(state,next_row,next_col));

  set_board_at(state,((state->snakes) + snum)->tail_row,((state->snakes) + snum)->tail_row,' ');


  (state->snakes + snum)->tail_col = next_col;
  (state->snakes + snum)->tail_row = next_row;

  set_board_at(state,((state->snakes) + snum)->tail_row,((state->snakes) + snum)->tail_col,tail_set);

  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  unsigned int snake_nums = state->num_snakes;

  for(unsigned int i = 0; i < snake_nums; i++){
    snake_t* snake = &state->snakes[i];
    
    if(snake->live == false){
      continue;
    }

    char next = next_square(state,i);

    if(is_snake(next) || next == '#'){
      snake->live = false;
      state->board[snake->head_row][snake->head_col] = 'x';
      continue;
    }

    if(next == '*'){
      update_head(state,i);
      add_food(state);
      continue;
    }

    update_head(state,i);
    update_tail(state,i);

  }

  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  char* line = NULL;

  size_t len = 0;
  ssize_t read;
  
  read = getline(&line,&len,fp);

  if(read == -1){
    return NULL;
  }


  char* heap_line = malloc(sizeof(char)*len + 5);

  if(heap_line == NULL){
    return NULL;
  }
  
  strcpy(heap_line,line);
  return heap_line;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  // TODO: Implement this function.

  game_state_t* start = (game_state_t*)malloc(sizeof(game_state_t));
  start->board = malloc(sizeof(start->board)*100);
  start->num_rows = 0;
  

  char* line;
  while((line = read_line(fp)) != NULL){

    size_t length = strlen(line);
    if (length > 0 && line[length - 1] == '\n') {
      line[length - 1] = '\0'; // Remove newline character
    }

    start->board = realloc(start->board,(start->num_rows + 1)* sizeof(char*));
    start->board[start->num_rows++] = line;
  }
  
  start->snakes = NULL;

  return start;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}
