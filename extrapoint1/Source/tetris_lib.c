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
volatile bool game_paused = true; // spec2

volatile char move_requested = MOVE_NONE;
uint16_t game_grid[GRID_ROWS][GRID_COLS] = {0};

const uint8_t tetrominoes[7][4][4] ={ // spec3
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


void init_leaderboard(){
    int y_pos = SCORE_YPOS;
    char text[256];
    sprintf(text, "TOP:");
    GUI_Text(SCORE_XPOS, SCORE_YPOS, (uint8_t*)text, Blue2, Black);

    sprintf(text, "%d", 0);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);


    sprintf(text, "SCORE:");
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, Blue2, Black);

    sprintf(text, "%d", 0);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);


    sprintf(text, "LINES:");
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, Blue2, Black);

    sprintf(text, "%d", 0);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);
}

void update_leaderboard(){
    int y_pos = SCORE_YPOS;
    char text[256];
    sprintf(text, "TOP:");
    GUI_Text(SCORE_XPOS, SCORE_YPOS, (uint8_t*)text, White, Black);

    sprintf(text, "%-10d", top_score);

    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);


    sprintf(text, "SCORE:");
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);

    sprintf(text, "%-10d", actual_score);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);


    sprintf(text, "LINES:");
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);

    sprintf(text, "%-10d", total_lines);
    y_pos += TEXT_WHITE_VERTICAL_SPACE;
    GUI_Text(SCORE_XPOS, y_pos, (uint8_t*)text, White, Black);
}

int get_actual_score(){
    return actual_score;
}

void set_actual_score(int new_actual_score){
    actual_score = new_actual_score;
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
    int i;
    // riempimento linee orizzontali per tutta l'altezza del blocco
    for (i = 0; i < TETROMINO_UNIT_BLOCK_SIZE; i++)
        LCD_DrawLine(x, y + i, x + TETROMINO_UNIT_BLOCK_SIZE - 1, y + i, color);

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
    new_tetromino.shape = rand() % 7;
    int colors[7] = {Cyan, Yellow, Magenta, Green, Red, Orange, Blue2};

    new_tetromino.color = colors[new_tetromino.shape];
    new_tetromino.pos_x = SPAWN_POINTX;

    if (new_tetromino.shape == TET_I) // align
        new_tetromino.pos_x -= TETROMINO_UNIT_BLOCK_SIZE;

    new_tetromino.pos_y = SPAWN_POINTY;
    new_tetromino.border = Black;
    new_tetromino.is_falling = true;

    if(check_collision(new_tetromino, new_tetromino.pos_x, new_tetromino.pos_y)){
        disable_timer(1);
    }
    return new_tetromino;
}

// non usat
bool is_falling_tetromino(Tetromino_t tetromino){
    return tetromino.is_falling;
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
                // Trasforma coordinate pixel in indici matrice
                int grid_x = (new_x - GAME_FIELD_LEFTX_LIMIT) / TETROMINO_UNIT_BLOCK_SIZE + i;
                int grid_y = (new_y - GAME_FIELD_UPY_LIMIT) / TETROMINO_UNIT_BLOCK_SIZE + j;

                // 1. Controlla i bordi del campo (X e Y)
                if (grid_x < 0 || grid_x >= GRID_COLS || grid_y >= GRID_ROWS) {
                    return true;
                }

                // 2. Controlla collisione con blocchi esistenti
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
    check_and_clear_lines();
    // redraw_game_field();
}

void check_and_clear_lines() {
    int r, c, count;
    for (r = GRID_ROWS - 1; r >= 0; r--) {
        count = 0;
        for (c = 0; c < GRID_COLS; c++) {
            if (game_grid[r][c] != 0) count++;
        }
        
        if (count == GRID_COLS) {
            // Riga completa! Bisogna scalarle tutte
            clear_line(r);
            r++; // Ricontrolla la stessa riga (perché quella sopra è scesa)
            
            // Aggiorna punteggio
            add_total_lines(1);
            set_actual_score(get_actual_score() + 100);
            update_leaderboard();
        }
    }
}


void clear_line(int row) {
    int r, c;

    // 1. Sposta logicamente tutte le righe sopra verso il basso
    for (r = row; r > 0; r--) {
        for (c = 0; c < GRID_COLS; c++) {
            game_grid[r][c] = game_grid[r-1][c];
        }
    }

    // 2. Svuota la riga più in alto (che ora è diventata libera)
    for (c = 0; c < GRID_COLS; c++) {
        game_grid[0][c] = 0;
    }

    // 3. Ridisegna l'area di gioco aggiornata
    redraw_game_field();
}


void redraw_game_field() {
    int r, c;
    for (r = 0; r < GRID_ROWS; r++) {
        for (c = 0; c < GRID_COLS; c++) {
            // Calcola la posizione pixel del blocco
            int x = GAME_FIELD_LEFTX_LIMIT + (c * TETROMINO_UNIT_BLOCK_SIZE);
            int y = GAME_FIELD_UPY_LIMIT + (r * TETROMINO_UNIT_BLOCK_SIZE);
            
            uint16_t color = game_grid[r][c];
            
            if (color != 0) {
                // Se c'è un blocco, usa la tua draw_block
                // Nota: potresti voler aggiungere un bordo per distinguere i blocchi fermi
                draw_block(x, y, color, Black); 
            } else {
                // Se è vuoto, disegna un quadrato nero per "cancellare" vecchi residui
                // (Assicurati che draw_block gestisca bene il riempimento)
                fill_rect(x, y, TETROMINO_UNIT_BLOCK_SIZE, TETROMINO_UNIT_BLOCK_SIZE, Black);
            }
        }
    }
}


void fill_rect(int x, int y, int width, int height, uint16_t color) {
    int i;
    for (i = 0; i < height; i++) {
        LCD_DrawLine(x, y + i, x + width - 1, y + i, color);
    }
}


void delete_tetromino(Tetromino_t tet) {
    int i, j;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            if (tetrominoes[tet.shape][j][i]) {
                int draw_x = tet.pos_x + (i * TETROMINO_UNIT_BLOCK_SIZE);
                int draw_y = tet.pos_y + (j * TETROMINO_UNIT_BLOCK_SIZE);

                fill_rect(draw_x, draw_y, TETROMINO_UNIT_BLOCK_SIZE, TETROMINO_UNIT_BLOCK_SIZE, Black);
            }
        }
    }
}

void handle_user_input(){
    if (move_requested == MOVE_NONE || game_paused) return;

    int next_x = falling_tetromino.pos_x;

    if(move_requested == MOVE_LEFT)
        next_x -= TETROMINO_UNIT_BLOCK_SIZE;
    else if(move_requested == MOVE_RIGHT)
        next_x += TETROMINO_UNIT_BLOCK_SIZE;

    if(!check_collision(falling_tetromino, next_x, falling_tetromino.pos_y)){
        delete_tetromino(falling_tetromino);
        falling_tetromino.pos_x = next_x;
        draw_tetromino(falling_tetromino);
    }
    move_requested = MOVE_NONE;
}



void perform_game_tick() {
    handle_user_input();
    
    delete_tetromino(falling_tetromino);
    int next_y = falling_tetromino.pos_y + TETROMINO_UNIT_BLOCK_SIZE; //spec5
    
    if (!check_collision(falling_tetromino, falling_tetromino.pos_x, next_y)) {
        falling_tetromino.pos_y = next_y;
        
    } else {
        draw_tetromino(falling_tetromino);
        lock_tetromino(falling_tetromino);
        check_and_clear_lines();
        falling_tetromino = generate_tetromino();
        // Game over con il pezzo appena creato?
    }
    
    draw_tetromino(falling_tetromino);
}

