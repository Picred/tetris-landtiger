#include "stdbool.h"
#include "time.h"
#include "stdlib.h"
#include "stdint.h"

// LCD
#define LCD_X_DIM 240
#define LCD_Y_DIM 320

// SCORE POSIITON
#define SCORE_XPOS ((LCD_X_DIM/2) + (LCD_X_DIM/5))
#define SCORE_YPOS 50
#define TEXT_WHITE_VERTICAL_SPACE 20

// PAUSED POSITION
#define PAUSED_TEXT_XPOS GAME_FIELD_RIGHTX_LIMIT + GAME_FIELD_EDGE_SIZE + 60
#define PAUSED_TEXT_YPOS (GAME_FIELD_HEIGTH / 2) + 50

// GAME FIELD
#define TOTAL_VERTICAL_BLOCKS 20 // spec1
#define TOTAL_HORIZONTAL_BLOCKS 10 // spec1
#define GAME_FIELD_EDGE_SIZE 5
#define TETRAMINOE_UNIT_BLOCK_SIZE 15

#define GAME_FIELD_UPY_LIMIT GAME_FIELD_EDGE_SIZE
#define GAME_FIELD_LEFTX_LIMIT GAME_FIELD_EDGE_SIZE
#define GAME_FIELD_DOWNY_LIMIT (TOTAL_VERTICAL_BLOCKS * TETRAMINOE_UNIT_BLOCK_SIZE) + GAME_FIELD_EDGE_SIZE
#define GAME_FIELD_RIGHTX_LIMIT (TOTAL_HORIZONTAL_BLOCKS * TETRAMINOE_UNIT_BLOCK_SIZE) + GAME_FIELD_EDGE_SIZE

#define GAME_FIELD_WIDTH (GAME_FIELD_RIGHTX_LIMIT - GAME_FIELD_LEFTX_LIMIT)
#define GAME_FIELD_HEIGTH (GAME_FIELD_DOWNY_LIMIT - GAME_FIELD_UPY_LIMIT)


#define SPAWN_POINTX (GAME_FIELD_LEFTX_LIMIT + (GAME_FIELD_WIDTH/2) - (TETRAMINOE_UNIT_BLOCK_SIZE))
#define SPAWN_POINTY GAME_FIELD_UPY_LIMIT

// LOGIC GAME FIELD
#define GRID_COLS (GAME_FIELD_WIDTH / TETRAMINOE_UNIT_BLOCK_SIZE)
#define GRID_ROWS (GAME_FIELD_HEIGTH / TETRAMINOE_UNIT_BLOCK_SIZE)

#define TETRAMINOE_FULL_WIDTH (4 * TETRAMINOE_UNIT_BLOCK_SIZE)
#define TETRAMINOE_FULL_HEIGTH (4 * TETRAMINOE_UNIT_BLOCK_SIZE)

#define TET_I 0
#define TET_O 1
#define TET_T 2
#define TET_S 3
#define TET_Z 4
#define TET_L 5
#define TET_J 6


// TETRAMINOES
typedef struct{
    int shape;
    int pos_x;
    int pos_y;
    int color;
    int border;
    bool is_falling;
} Tetraminoe_t;


Tetraminoe_t generate_tetraminoe();
void draw_tetraminoe(Tetraminoe_t tetraminoe);
bool is_falling_tetraminoe(Tetraminoe_t tetraminoe);

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
bool check_collision(Tetraminoe_t tet, int new_x, int new_y);
void lock_tetraminoe(Tetraminoe_t tet);
void check_and_clear_lines();
void redraw_game_field();
void clear_line(int row);
void perform_game_tick();
void delete_tetraminoe(Tetraminoe_t tet);
void fill_rect(int x, int y, int width, int height, uint16_t color);