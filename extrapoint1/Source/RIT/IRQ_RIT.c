#include "RIT.h"
#include "LPC17xx.h"

#include "../timer/timer.h"
#include "../joystick/joystick.h"
#include "../button_EXINT/button.h"
#include "./tetris.h"


volatile uint32_t pressed_button_0 = 0;
volatile uint32_t pressed_button_1 = 0;
volatile uint32_t pressed_button_2 = 0;

volatile uint32_t pressed_joystick_up = 0;
volatile uint32_t pressed_joystick_left = 0;
volatile uint32_t pressed_joystick_right = 0;
volatile uint32_t pressed_joystick_down = 0;
volatile uint32_t pressed_joystick_select = 0;
volatile uint32_t pressed_joystick_up_left = 0;
volatile uint32_t pressed_joystick_up_right = 0;
volatile uint32_t pressed_joystick_down_left = 0;
volatile uint32_t pressed_joystick_down_right = 0;

volatile uint8_t joystick_flag = 0;
volatile uint8_t btn_flag = 0;

extern volatile bool game_paused;
extern volatile bool game_over;
extern volatile bool game_reset_requested;

extern volatile Tetromino_t falling_tetromino;
extern volatile char move_requested;


void RIT_IRQHandler(void){    
    // -------------------------------
    // JOYSTICK UP
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_UP)){
        pressed_joystick_up++;
        if(pressed_joystick_up >= 1) {
            // MY code here :)
            move_requested = MOVE_UP;
            joystick_flag |= FLAG_JOYSTICK_UP;
        }
    }
    else pressed_joystick_up = 0;

    // -------------------------------
    // JOYSTICK DOWN
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_DOWN)){
        pressed_joystick_down++;
        if(pressed_joystick_down >= 1) {
            // MY code here :)
            move_requested = MOVE_DOWN;
            joystick_flag |= FLAG_JOYSTICK_DOWN;
        }
    }
    else{
        pressed_joystick_down = 0;
        falling_tetromino.drop_speed = NORMAL_DROP_SPEED;

    }

    // -------------------------------
    // JOYSTICK LEFT
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_LEFT)){
        pressed_joystick_left++;
        if(pressed_joystick_left >= 1) {
            // code
            move_requested = MOVE_LEFT;
            joystick_flag |= FLAG_JOYSTICK_LEFT;
        }
    }
    else pressed_joystick_left = 0;

    // -------------------------------
    // JOYSTICK RIGHT
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_RIGHT)){
        pressed_joystick_right++;
        if(pressed_joystick_right >= 1) {
            // MY code here :)
            move_requested = MOVE_RIGHT;
            joystick_flag |= FLAG_JOYSTICK_RIGHT;
        }
    }
    else pressed_joystick_right = 0;


    // -------------------------------
    // JOYSTICK SELECT
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_PRESS)){
        pressed_joystick_select++;
        if(pressed_joystick_select == 1) {
            // MY code here :)
            joystick_flag |= FLAG_JOYSTICK_SELECT;
        }
    }
    else pressed_joystick_select = 0;

    // -------------------------------
    // JOYSTICK UP LEFT
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_UP)){
        pressed_joystick_up_left++;
        if(pressed_joystick_up_left == 1) {
            // MY code here :)
            joystick_flag |= FLAG_JOYSTICK_UP;
        }
    }
    else pressed_joystick_up_left = 0;

    // -------------------------------
    // JOYSTICK UP RIGHT
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_UP)){
        pressed_joystick_up_right++;
        if(pressed_joystick_up_right == 1) {
            // MY code here :)
            joystick_flag |= FLAG_JOYSTICK_UP;
        }
    }
    else pressed_joystick_up_right = 0;

    // -------------------------------
    // JOYSTICK DOWN LEFT
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_DOWN)){
        pressed_joystick_down_left++;
        if(pressed_joystick_down_left == 1) {
            // MY code here :)
            joystick_flag |= FLAG_JOYSTICK_DOWN;
        }
    }
    else pressed_joystick_down_left = 0;

    // -------------------------------
    // JOYSTICK DOWN
    // -------------------------------

    if(joystick_check_dir(JOYSTICK_DOWN)){
        pressed_joystick_down_right++;
        if(pressed_joystick_down_right == 1) {
            // MY code here :)
            joystick_flag |= FLAG_JOYSTICK_DOWN;
        }
    }
    else pressed_joystick_down_right = 0;

    // -------------------------------
    // BUTTON 0
    // -------------------------------

    if(pressed_button_0 != 0){
        if(LPC_GPIO2->FIOPIN & (1 << 10)){
            if(pressed_button_0 >= SHORT_PRESS_COUNT){
                // short press
                btn_flag |= FLAG_BUTTON_0_SHORT;
            }
            pressed_button_0 = 0;
            NVIC_EnableIRQ(EINT0_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 20);
        } else {
            if(pressed_button_0 >= LONG_PRESS_COUNT){
                // long press
                btn_flag |= FLAG_BUTTON_0_LONG;
                pressed_button_0 = 0;
            }
            pressed_button_0++;
        }
    }

    // -------------------------------
    // BUTTON 1
    // -------------------------------

    if(pressed_button_1 != 0){
        if(LPC_GPIO2->FIOPIN & (1 << 11)){
            if(pressed_button_1 >= SHORT_PRESS_COUNT){
                // short press 
                btn_flag |= FLAG_BUTTON_1_SHORT;
            }
            if(!game_over){
                game_paused = !game_paused;
                print_or_delete_paused_text();
            }else{
                game_over = false;
                game_paused = true;
                game_reset_requested = true;
            }
            
            
            pressed_button_1 = 0;
            NVIC_EnableIRQ(EINT1_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 22);
        } 
        else {
            if(pressed_button_1 >= LONG_PRESS_COUNT){
                // long press
                btn_flag |= FLAG_BUTTON_1_LONG;
                pressed_button_1 = 0;
            }
            pressed_button_1++;
        }
    }

    // -------------------------------
    // BUTTON 2
    // -------------------------------

    if(pressed_button_2 != 0){
        if(LPC_GPIO2->FIOPIN & (1 << 12)){
            if(pressed_button_2 >= SHORT_PRESS_COUNT){
                // short press
                btn_flag |= FLAG_BUTTON_2_SHORT;
            }
            move_requested = MOVE_HARD_DROP;
            pressed_button_2 = 0;
            LPC_PINCON->PINSEL4 &= ~(3 << 24);
            LPC_PINCON->PINSEL4 |= (1 << 24);
            NVIC_EnableIRQ(EINT2_IRQn);
        } else {
            if(pressed_button_2 >= LONG_PRESS_COUNT){
                // long press
                btn_flag |= FLAG_BUTTON_2_LONG;
                pressed_button_2 = 0;
            }
            pressed_button_2++;
        }
    }

    // -------------------------------
    // ADC Conversion
    // -------------------------------

    // ADC_start_conversion(); // LEGGE IL VALORE DEL POTENZIOMETRO
    LPC_RIT->RICTRL |= 0x1;
}
