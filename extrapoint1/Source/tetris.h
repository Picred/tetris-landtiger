#include "stdbool.h"
#include "time.h"
#include "stdlib.h"
#include "stdint.h"

// LCD
#define LCD_X_DIM 240
#define LCD_Y_DIM 320

// SCORE POSIITON
#define SCORE_XPOS ((LCD_X_DIM/2) + (LCD_X_DIM/5))
#define SCORE_YPOS 30
#define TEXT_WHITE_VERTICAL_SPACE 20

// PAUSED POSITION
#define PAUSED_TEXT_XPOS SCORE_XPOS
#define PAUSED_TEXT_YPOS SCORE_YPOS + (7*TEXT_WHITE_VERTICAL_SPACE)

// GAME FIELD
#define TOTAL_VERTICAL_BLOCKS 20 // spec1
#define TOTAL_HORIZONTAL_BLOCKS 10 // spec1
#define GAME_FIELD_EDGE_SIZE 5
#define TETROMINO_UNIT_BLOCK_SIZE 15 // max 15, min 3

#define GAME_FIELD_UPY_LIMIT GAME_FIELD_EDGE_SIZE
#define GAME_FIELD_LEFTX_LIMIT GAME_FIELD_EDGE_SIZE
#define GAME_FIELD_DOWNY_LIMIT (TOTAL_VERTICAL_BLOCKS * TETROMINO_UNIT_BLOCK_SIZE) + GAME_FIELD_EDGE_SIZE
#define GAME_FIELD_RIGHTX_LIMIT (TOTAL_HORIZONTAL_BLOCKS * TETROMINO_UNIT_BLOCK_SIZE) + GAME_FIELD_EDGE_SIZE

#define GAME_FIELD_WIDTH (GAME_FIELD_RIGHTX_LIMIT - GAME_FIELD_LEFTX_LIMIT)
#define GAME_FIELD_HEIGTH (GAME_FIELD_DOWNY_LIMIT - GAME_FIELD_UPY_LIMIT)


#define SPAWN_POINTX (GAME_FIELD_LEFTX_LIMIT + (GAME_FIELD_WIDTH/2) - (TETROMINO_UNIT_BLOCK_SIZE))
#define SPAWN_POINTY GAME_FIELD_UPY_LIMIT

// LOGIC GAME FIELD
#define GRID_COLS (GAME_FIELD_WIDTH / TETROMINO_UNIT_BLOCK_SIZE)
#define GRID_ROWS (GAME_FIELD_HEIGTH / TETROMINO_UNIT_BLOCK_SIZE)

#define TETROMINO_FULL_WIDTH (4 * TETROMINO_UNIT_BLOCK_SIZE)
#define TETROMINO_FULL_HEIGTH (4 * TETROMINO_UNIT_BLOCK_SIZE)

#define TET_I 0
#define TET_O 1
#define TET_T 2
#define TET_S 3
#define TET_Z 4
#define TET_L 5
#define TET_J 

#define MOVE_LEFT 10
#define MOVE_RIGHT 11
#define MOVE_NONE 12


// TETROMINOS
typedef struct{
    int shape;
    int pos_x;
    int pos_y;
    int color;
    int border;
    bool is_falling;
} Tetromino_t;


Tetromino_t generate_tetromino();
bool is_falling_tetromino(Tetromino_t tetromino);
void handle_user_input();
void perform_game_tick();
void delete_tetromino(Tetromino_t tet);
void check_and_clear_lines();
bool check_collision(Tetromino_t tet, int new_x, int new_y);
void lock_tetromino(Tetromino_t tet);



// UTILS
void print_screen(uint16_t Xpos, uint16_t Ypos, char* str, uint16_t Color, uint16_t bkColor);
void print_or_delete_paused_text();


/* LEADERBOARD*/
void init_leaderboard();
void update_leaderboard();
int get_actual_score();
int get_total_lines();
void set_actual_score(int new_actual_score);
void set_top_score(int new_top_score);
int get_top_score();
void add_total_lines(int lines_to_add);
void reset_total_lines();

/* GAME FIELD */
void init_game_field();
void draw_tetromino(Tetromino_t tetromino);
void redraw_game_field();
void clear_line(int row);
void draw_rect(int x, int y, int width, int height, uint16_t color);