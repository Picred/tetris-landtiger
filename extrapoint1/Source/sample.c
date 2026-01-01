#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "timer/timer.h"

#include "./tetris.h"
#include "stdio.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

extern volatile Tetromino_t falling_tetromino;
extern volatile bool game_paused;

int main(void) {
    SystemInit(); /* System Initialization (i.e., PLL)  */
    LCD_Initialization();
    BUTTON_init();
    joystick_init();
    init_RIT(250500); // 2.5 ms
    enable_RIT();

    init_timer(0, 0, 0, 3, 2500000);
    enable_timer(0);
    init_timer(1, 0, 0, 3, 250500); // 2.5 ms
    
    LCD_Clear(Black);
    update_leaderboard();
    init_game_field();
    print_or_delete_paused_text();
    
    enable_timer(1);
    while(game_paused);
    srand(LPC_TIM0->TC);
    disable_timer(0);
    falling_tetromino = generate_tetromino();

    LPC_SC->PCON |= 0x1; /* power-down    mode */
    LPC_SC->PCON &= ~(0x2);

    while (1) {
        __ASM("wfi");

    }
}
