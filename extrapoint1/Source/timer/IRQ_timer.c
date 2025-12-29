#include "LPC17xx.h"
#include "timer.h"
#include "./tetris.h"
#include "GLCD/GLCD.h"

volatile int y_axis = 0;
extern volatile int colors[];
extern volatile Tetromino_t falling_tetromino;
extern volatile bool game_paused;


void TIMER0_IRQHandler(void) { // used for srand on main
	if (LPC_TIM0->IR & 01) {
	}
	LPC_TIM0->IR = 1;   /* clear interrupt flag */
	return;
}


void TIMER1_IRQHandler(void) { // time game 
	if (LPC_TIM1->IR & 01) {
        if(!game_paused)
            perform_game_tick();
    }
    LPC_TIM1->IR = 1;   /* clear interrupt flag */
	return;
}



void TIMER2_IRQHandler(void) {
	if (LPC_TIM2->IR & 01) {

	}
	LPC_TIM2->IR = 1; /* clear interrupt flag */
	return;
}
