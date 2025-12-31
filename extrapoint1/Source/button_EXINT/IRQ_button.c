#include "button.h"
#include "LPC17xx.h"
#include "./tetris.h"
#include "stdio.h"

extern volatile bool game_paused;
extern volatile uint32_t pressed_button_0;
extern volatile uint32_t pressed_button_1;
extern volatile uint32_t pressed_button_2;


// KEY0
void EINT0_IRQHandler (void){        
    LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt */
}

// KEY1 test RIT
void EINT1_IRQHandler (void){
    pressed_button_1 = 1;
    NVIC_DisableIRQ(EINT1_IRQn); /* disable Button interrupts */
    LPC_PINCON->PINSEL4    &= ~(1 << 22); /* GPIO pin selection */
    LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt */
}


// KEY2
void EINT2_IRQHandler (void){
    LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt */
}
