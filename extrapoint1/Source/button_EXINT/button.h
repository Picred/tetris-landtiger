void BUTTON_init(void);

void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

#define BUTTON_0 0
#define BUTTON_1 1
#define BUTTON_2 2

#define FLAG_BUTTON_0_SHORT (1 << BUTTON_0)
#define FLAG_BUTTON_1_SHORT (1 << BUTTON_1)
#define FLAG_BUTTON_2_SHORT (1 << BUTTON_2)
#define FLAG_BUTTON_0_LONG     ((1 << BUTTON_0) << 3)
#define FLAG_BUTTON_1_LONG  ((1 << BUTTON_1) << 3)
#define FLAG_BUTTON_2_LONG  ((1 << BUTTON_2) << 3)