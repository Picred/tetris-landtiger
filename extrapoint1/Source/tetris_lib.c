#include "./tetris.h"
#include "GLCD.h"
#include "string.h"
#include "stdio.h"
#include "timer/timer.h"

// globals
volatile int top_score = 0;
volatile int actual_score = 0;
volatile int total_lines = 0;
volatile Tetromino_t falling_tetromino;
volatile bool game_paused = true;
volatile bool game_over = false;
volatile bool game_reset_requested = false;
volatile int colors[19] = {Cyan, Yellow, Magenta, Green, Red, Orange, Blue2, Cyan, Magenta, Magenta, Magenta, Green, Red, Orange, Orange, Orange, Blue2, Blue2, Blue2 };

volatile char move_requested = MOVE_NONE;
uint16_t game_grid[GRID_ROWS][GRID_COLS] = {0};


const uint8_t tetrominoes[19][4][4] ={ // spec3
    // I
    {
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // O
    {
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // T
    {
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // S
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // Z
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // L
    {
        {1,0,0,0},
        {1,0,0,0},
        {1,1,0,0},
        {0,0,0,0}
    },
    // J
    {
        {0,1,0,0},
        {0,1,0,0},
        {1,1,0,0},
        {0,0,0,0}
    },
    // I_90
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
    },
    // T_90
    {
        {0,1,0,0},
        {1,1,0,0},
        {0,1,0,0},
        {0,0,0,0}
    },
    // T_180
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // T_270
    {
        {1,0,0,0},
        {1,1,0,0},
        {1,0,0,0},
        {0,0,0,0}
    },
    // S_90
    {
        {1,0,0,0},
        {1,1,0,0},
        {0,1,0,0},
        {0,0,0,0}
    },
    // Z_90
    {
        {0,1,0,0},
        {1,1,0,0},
        {1,0,0,0},
        {0,0,0,0}
    },
    // L_90
    {
        {1,1,1,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // L_180
    {
        {1,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,0,0,0}
    },
    // L_270
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // J_90
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // J_180
    {
        {1,1,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0}
    },
    // J_270
    {
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }

};


void print_or_delete_paused_text(){
    char paused_text[64];
    sprintf(paused_text, "PAUSED");

    int backround_color;
    if(game_paused)
        backround_color = White;
    else
        backround_color = Black;

    GUI_Text(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS, (uint8_t*)paused_text, Black, backround_color);
}

void print_or_delete_game_over_text(){
    char game_over_text[64];
    sprintf(game_over_text, "GAME OVER");

    int text_color;
    if(game_over)
        text_color = Red;
    else
        text_color = Black;

    GUI_Text(GAME_OVER_TEXT_XPOS, GAME_OVER_TEXT_YPOS, (uint8_t*)game_over_text, text_color, Black);
}


void update_leaderboard(){
    int y_pos = SCORE_YPOS;
    char text[256];
    sprintf(text, "TOP:");
    GUI_Text(SCORE_XPOS, SCORE_YPOS, (uint8_t*)text, Blue2, Black);

    sprintf(text, "%5d", top_score);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);


    sprintf(text, "SCORE:");
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, Blue2, Black);

    sprintf(text, "%5d", actual_score);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);


    sprintf(text, "LINES:");
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, Blue2, Black);

    sprintf(text, "%5d", total_lines);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);
}

int get_actual_score(){
    return actual_score;
}

void set_actual_score(int new_actual_score){
    actual_score = new_actual_score;
}

void reset_actual_score(){
    actual_score = 0;
}

int get_total_lines(){
    return total_lines;
}

void add_total_lines(int lines_to_add){
    total_lines += lines_to_add;
}

void reset_total_lines(){
    total_lines = 0;
}

int get_top_score(){
    return top_score;
}

void set_top_score(int new_top_score){
    top_score = new_top_score;
}

void calculate_new_top_score(){
    top_score = (actual_score > top_score ? actual_score : top_score);
}


void init_game_field() {
    int i;
    for(i = 0; i < GAME_FIELD_EDGE_SIZE; i++) {
        // up
        LCD_DrawLine(GAME_FIELD_LEFTX_LIMIT - GAME_FIELD_EDGE_SIZE, i, GAME_FIELD_RIGHTX_LIMIT + GAME_FIELD_EDGE_SIZE - 1, i, White);

        // down
        LCD_DrawLine(GAME_FIELD_LEFTX_LIMIT - GAME_FIELD_EDGE_SIZE, GAME_FIELD_DOWNY_LIMIT + i, GAME_FIELD_RIGHTX_LIMIT + GAME_FIELD_EDGE_SIZE - 1, GAME_FIELD_DOWNY_LIMIT + i, White);

        // left
        LCD_DrawLine(i, GAME_FIELD_UPY_LIMIT, i, GAME_FIELD_DOWNY_LIMIT - 1, White);

        // right
        LCD_DrawLine(GAME_FIELD_RIGHTX_LIMIT + i, GAME_FIELD_UPY_LIMIT, GAME_FIELD_RIGHTX_LIMIT + i, GAME_FIELD_DOWNY_LIMIT - 1, White);
    }
}

void draw_block(int x, int y, short color, short border_color) {
    // int i;

    draw_rect(x+1, y+1, TETROMINO_UNIT_BLOCK_SIZE -2, TETROMINO_UNIT_BLOCK_SIZE -2, color);
    //bordo rettangolo vuoto sopra il riempimento
    LCD_DrawLine(x, y, x + TETROMINO_UNIT_BLOCK_SIZE - 1, y, border_color); // up
    LCD_DrawLine(x, y + TETROMINO_UNIT_BLOCK_SIZE - 1, x + TETROMINO_UNIT_BLOCK_SIZE - 1, y + TETROMINO_UNIT_BLOCK_SIZE - 1, border_color); // down
    LCD_DrawLine(x, y, x, y + TETROMINO_UNIT_BLOCK_SIZE - 1, border_color); // sx
    LCD_DrawLine(x + TETROMINO_UNIT_BLOCK_SIZE - 1, y, x + TETROMINO_UNIT_BLOCK_SIZE - 1, y + TETROMINO_UNIT_BLOCK_SIZE - 1, border_color); // dx
}


void draw_tetromino(Tetromino_t tetromino) {
    int i, j;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            if (tetrominoes[tetromino.shape][j][i]) { // Se nella matrice 4x4 c'è un pezzo (1), lo disegno
                int draw_x = tetromino.pos_x + (i * TETROMINO_UNIT_BLOCK_SIZE);
                int draw_y = tetromino.pos_y + (j * TETROMINO_UNIT_BLOCK_SIZE);

                draw_block(draw_x, draw_y, tetromino.color, tetromino.border);
            }
        }
    }
}

// spec4
Tetromino_t generate_tetromino(){
    Tetromino_t new_tetromino;
    new_tetromino.shape = rand() % 19;

    new_tetromino.color = colors[new_tetromino.shape];
    new_tetromino.pos_x = SPAWN_POINTX;
    new_tetromino.pos_y = SPAWN_POINTY;

    if(check_collision(new_tetromino, new_tetromino.pos_x, new_tetromino.pos_y)){
        game_over = true;
        print_or_delete_game_over_text();
        calculate_new_top_score();
        update_leaderboard();
        // print_screen(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS, "GAME OVER", Red, Black);
    }

    if (new_tetromino.shape == TET_I) // align
        new_tetromino.pos_x -= TETROMINO_UNIT_BLOCK_SIZE;

    new_tetromino.border = Black;
    new_tetromino.drop_speed = NORMAL_DROP_SPEED;

    return new_tetromino;
}



void print_screen(uint16_t Xpos, uint16_t Ypos, char* str, uint16_t color, uint16_t bkColor){
    char text[64];
    sprintf(text, "%s", str);
    GUI_Text(Xpos, Ypos, (uint8_t *)text, color, bkColor);
}


//logic
bool check_collision(Tetromino_t tet, int new_x, int new_y) {
    int i, j;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            if (tetrominoes[tet.shape][j][i]) {
                // coordinate pixel -> indici matrice
                int grid_x = (new_x - GAME_FIELD_LEFTX_LIMIT) / TETROMINO_UNIT_BLOCK_SIZE + i;
                int grid_y = (new_y - GAME_FIELD_UPY_LIMIT) / TETROMINO_UNIT_BLOCK_SIZE + j;

                // check bordi del GAME_FIELD (X e Y)
                if (grid_x < 0 || grid_x >= GRID_COLS || grid_y >= GRID_ROWS) {
                    return true;
                }

                // check collisione con altri blocchi (game_grid [i][j] != 0)
                if (game_grid[grid_y][grid_x] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}


void lock_tetromino(Tetromino_t tet) {
    int i, j;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            if (tetrominoes[tet.shape][j][i]) {
                int grid_x = (tet.pos_x - GAME_FIELD_LEFTX_LIMIT) / TETROMINO_UNIT_BLOCK_SIZE + i;
                int grid_y = (tet.pos_y - GAME_FIELD_UPY_LIMIT) / TETROMINO_UNIT_BLOCK_SIZE + j;

                if (grid_y >= 0 && grid_y < GRID_ROWS) {
                    game_grid[grid_y][grid_x] = tet.color;
                }
            }
        }
    }
    set_actual_score(get_actual_score() + 10);
    update_leaderboard();
}

void check_and_clear_lines() {
    int i, j, num_blocks;
    int total_cleared_lines = 0;
    int total_bonus = 0;
    for (i = GRID_ROWS - 1; i >= 0; i--) {
        num_blocks = 0;
        for (j = 0; j < GRID_COLS; j++) {
            if (game_grid[i][j] != 0) num_blocks++;
        }

        if (num_blocks == GRID_COLS) { // riga completa, tutte le colonne "contate"
            clear_line(i);
            i++; // re-check stessa riga
            total_cleared_lines++;
        }
    }
    if (total_cleared_lines == 4){
        total_bonus = TETRIS_BONUS;
        print_screen(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS + TEXT_WHITE_VERTICAL_SPACE, "!TETRIS!", Orange, Black);
        print_screen(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS + TEXT_WHITE_VERTICAL_SPACE, "!TETRIS!", Black, Black);
        print_screen(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS + TEXT_WHITE_VERTICAL_SPACE, "!TETRIS!", Orange, Black);
        print_screen(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS + TEXT_WHITE_VERTICAL_SPACE, "!TETRIS!", Black, Black);
        print_screen(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS + TEXT_WHITE_VERTICAL_SPACE, "!TETRIS!", Orange, Black);
        print_screen(PAUSED_TEXT_XPOS, PAUSED_TEXT_YPOS + TEXT_WHITE_VERTICAL_SPACE, "!TETRIS!", Black, Black);
    }
    else if (total_cleared_lines > 0)
        total_bonus = 100 * total_cleared_lines;

    set_actual_score(get_actual_score() + total_bonus);
    add_total_lines(total_cleared_lines);
    update_leaderboard();
}



void redraw_partial_field(int start_row) {
    int i, j;
    for (i = start_row; i >= 0; i--) {
        for (j = 0; j < GRID_COLS; j++) {
            int x = GAME_FIELD_LEFTX_LIMIT + (j * TETROMINO_UNIT_BLOCK_SIZE);
            int y = GAME_FIELD_UPY_LIMIT + (i * TETROMINO_UNIT_BLOCK_SIZE);

            uint16_t color = game_grid[i][j];

            if (color != 0) {
                draw_block(x, y, color, Black); // Border color coerente
            } else {
                draw_rect(x, y, TETROMINO_UNIT_BLOCK_SIZE, TETROMINO_UNIT_BLOCK_SIZE, Black);
            }
        }
    }
}

void clear_line(int row) {
    int i, j;
    for (i = row; i > 0; i--) {
        for (j = 0; j < GRID_COLS; j++) {
            game_grid[i][j] = game_grid[i-1][j];
        }
    }
    for (j = 0; j < GRID_COLS; j++) {
        game_grid[0][j] = 0;
    }
    redraw_partial_field(row); 
}


void draw_rect(int x, int y, int width, int height, uint16_t color) {
    if (x >= MAX_X || y >= MAX_Y) return;
    LCD_DrawEntireSquare(x, y, width, height, color);
}

void delete_tetromino(Tetromino_t tet) {
    int i, j;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            if (tetrominoes[tet.shape][j][i]) {
                int draw_x = tet.pos_x + (i * TETROMINO_UNIT_BLOCK_SIZE);
                int draw_y = tet.pos_y + (j * TETROMINO_UNIT_BLOCK_SIZE);

                draw_rect(draw_x, draw_y, TETROMINO_UNIT_BLOCK_SIZE, TETROMINO_UNIT_BLOCK_SIZE, Black);
            }
        }
    }
}

void rotate_falling_tetromino(){
    int original_shape = falling_tetromino.shape;
    int next_shape = original_shape;

    switch (original_shape){
        case TET_I: next_shape = TET_I_90; 
            break;
        case TET_I_90: next_shape = TET_I;
            break;

        case TET_T: next_shape = TET_T_90;
            break;
        case TET_T_90: next_shape = TET_T_180;
            break;
        case TET_T_180: next_shape = TET_T_270;
            break;
        case TET_T_270: next_shape = TET_T;
            break;

        case TET_S: next_shape = TET_S_90;
            break;
        case TET_S_90: next_shape = TET_S;
            break;

        case TET_Z: next_shape = TET_Z_90;
            break;
        case TET_Z_90: next_shape = TET_Z;
            break;
        
        case TET_L: next_shape = TET_L_90;
            break;
        case TET_L_90: next_shape = TET_L_180;
            break;
        case TET_L_180: next_shape = TET_L_270;
            break;
        case TET_L_270: next_shape = TET_L;
            break;

        case TET_J: next_shape = TET_J_90;
            break;
        case TET_J_90: next_shape = TET_J_180;
            break;
        case TET_J_180: next_shape = TET_J_270;
            break;
        case TET_J_270: next_shape = TET_J;
            break;
    }
    falling_tetromino.shape = next_shape;
    if(check_collision(falling_tetromino, falling_tetromino.pos_x, falling_tetromino.pos_y))
        falling_tetromino.shape = original_shape; // se collide la nuova forma, ritorna quella prima
}

void hard_drop_falling_tetromino(){
    delete_tetromino(falling_tetromino);
    while (!check_collision(falling_tetromino, falling_tetromino.pos_x, falling_tetromino.pos_y + TETROMINO_UNIT_BLOCK_SIZE)) {
        falling_tetromino.pos_y += TETROMINO_UNIT_BLOCK_SIZE;
    }
}

void handle_user_input(){
    if (move_requested == MOVE_NONE || game_paused || game_over) return;

    int next_x = falling_tetromino.pos_x;

    switch (move_requested){
        case MOVE_LEFT:
            next_x -= TETROMINO_UNIT_BLOCK_SIZE;
            break;
        case MOVE_RIGHT:
            next_x += TETROMINO_UNIT_BLOCK_SIZE;
            break;
        case MOVE_DOWN: // soft drop
            falling_tetromino.drop_speed = SOFT_DROP_SPEED;
            break;
        case MOVE_UP: //rotate
            delete_tetromino(falling_tetromino);
            rotate_falling_tetromino();
            break;
        case MOVE_HARD_DROP:
            hard_drop_falling_tetromino();
            break;
    }
    if(!check_collision(falling_tetromino, next_x, falling_tetromino.pos_y)){
        delete_tetromino(falling_tetromino);
        falling_tetromino.pos_x = next_x;
        draw_tetromino(falling_tetromino);
    }
    move_requested = MOVE_NONE;
}


void perform_game_tick() {
    handle_user_input(); // race conditions
    int next_y = falling_tetromino.pos_y + (falling_tetromino.drop_speed*TETROMINO_UNIT_BLOCK_SIZE); //spec5

    if (!check_collision(falling_tetromino, falling_tetromino.pos_x, next_y)) {
        delete_tetromino(falling_tetromino);
        falling_tetromino.pos_y = next_y;
        draw_tetromino(falling_tetromino);

    } else {
        // di 2 non puo scendere, ma di 1?
        int single_step_y = falling_tetromino.pos_y + TETROMINO_UNIT_BLOCK_SIZE;
        while (!check_collision(falling_tetromino, falling_tetromino.pos_x, falling_tetromino.pos_y + TETROMINO_UNIT_BLOCK_SIZE)) {
            delete_tetromino(falling_tetromino);
            falling_tetromino.pos_y += TETROMINO_UNIT_BLOCK_SIZE; // incremento finchè non c'è collisione - max 2
        }
        draw_tetromino(falling_tetromino);
        lock_tetromino(falling_tetromino);
        check_and_clear_lines();
        falling_tetromino = generate_tetromino();
        draw_tetromino(falling_tetromino);
    }
}

void reset_game(){
    LCD_DrawEntireSquare(GAME_FIELD_LEFTX_LIMIT, GAME_FIELD_UPY_LIMIT, GAME_FIELD_WIDTH, GAME_FIELD_HEIGTH, Black);
    print_or_delete_paused_text();
    game_over = false;
    print_or_delete_game_over_text();
    memset(game_grid, 0, sizeof(game_grid));
    reset_total_lines();
    reset_actual_score();
    update_leaderboard();
}

