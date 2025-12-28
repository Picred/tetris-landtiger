/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	// joy sel
	LPC_PINCON->PINSEL3 &= ~(3<<18);	//PIN mode GPIO (00b value per P1.25)
	LPC_GPIO1->FIODIR   &= ~(1<<25);	//P1.25 Input (joysticks on PORT1 defined as Input)
	
	// joy down
	LPC_PINCON->PINSEL3 &= ~(3<<20);	//PIN mode GPIO (00b value per P1.24)
	LPC_GPIO1->FIODIR   &= ~(1<<26);	//P1.25 Input (joysticks on PORT1 defined as Input)
	
	// joy left
	LPC_PINCON->PINSEL3 &= ~(3<<22);	//PIN mode GPIO (00b value per P1.23)
	LPC_GPIO1->FIODIR   &= ~(1<<27);	//P1.25 Input (joysticks on PORT1 defined as Input)
	
	// joy right 
	LPC_PINCON->PINSEL3 &= ~(3<<24);	//PIN mode GPIO (00b value per P1.22)
	LPC_GPIO1->FIODIR   &= ~(1<<28);	//P1.25 Input (joysticks on PORT1 defined as Input)
	
	// joy up
	LPC_PINCON->PINSEL3 &= ~(3<<26);	//PIN mode GPIO (00b value per P1.21)
	LPC_GPIO1->FIODIR   &= ~(1<<29);	//P1.25 Input (joysticks on PORT1 defined as Input)
  
}


int joystick_check_dir(uint32_t dir){
	if ((LPC_GPIO1->FIOPIN & ( 1 << dir)) == 0) return 1;
	return 0;
}