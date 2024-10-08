#include "lpc17xx.h"
#include "timer.h"
#include "../button_exint/button.h"
#include "../led/led.h"

extern int down_0;
extern int down_1;
extern int down_2;

void TIMER0_IRQHandler (void)
{
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
	return;
}

void TIMER1_IRQHandler (void)
{
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
	return;
}

void TIMER3_IRQHandler (void)
{
	
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}