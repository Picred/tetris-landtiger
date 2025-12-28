#ifndef __TIMER_H
#define __TIMER_H

//extern uint32_t init_timer( uint8_t timer_num, uint32_t timerInterval );
extern uint32_t init_timer(uint8_t timer_num, uint32_t Prescaler, uint8_t MatchReg, uint8_t SRImatchReg, uint32_t TimerInterval);

extern void enable_timer(uint8_t timer_num);
extern void disable_timer(uint8_t timer_num);
extern void reset_timer(uint8_t timer_num);

/* IRQ_timer.c */
extern void TIMER0_IRQHandler(void);
extern void TIMER1_IRQHandler(void);

#endif
