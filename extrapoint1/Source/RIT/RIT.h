/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           RIT.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_RIT, funct_RIT, IRQ_RIT .c files
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __RIT_H
#define __RIT_H


#include "stdint.h"

/* init_RIT.c */
extern uint32_t init_RIT( uint32_t RITInterval );
extern void enable_RIT( void );
extern void disable_RIT( void );
extern void reset_RIT( void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);

#define SHORT_PRESS_MS 300U
#define LONG_PRESS_MS 1000U
#define RIT_PERIOD_MS 50U

#define SHORT_PRESS_COUNT (SHORT_PRESS_MS / RIT_PERIOD_MS)
#define LONG_PRESS_COUNT (LONG_PRESS_MS / RIT_PERIOD_MS)

#endif
