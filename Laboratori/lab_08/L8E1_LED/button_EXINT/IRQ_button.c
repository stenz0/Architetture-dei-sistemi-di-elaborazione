#include "button.h"
#include "lpc17xx.h"
#include "../led/led.h"

int count = 0;

void EINT0_IRQHandler (void)	  
{
	count = 0;
	LED_Out(count);
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  
{
	count++;
  LED_Out(count);
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{
	count--;
	LED_Out(count);
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


