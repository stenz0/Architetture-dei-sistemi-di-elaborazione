#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../button_exint/button.h"
#include "../timer/timer.h"
#include "../led/led.h"

volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;

void RIT_IRQHandler (void)
{
	static int J_select = 0;
	static int J_down = 0;
	static int J_high = 0;
	static int J_left = 0;
	static int J_right = 0;	
		
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		
		J_select++;
		switch(J_select){
			case 1: 
				
				break;
			
			default:
				break;
		}
	}
	else{
			J_select = 0;
	}
	
	
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		
		J_down++;
		switch(J_down){
			case 1: 
							
				break;
			
			default:
				break;
		}
	}
	else{
			J_down = 0;
	}
	
	
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		
		J_high++;
		switch(J_high){
			case 1:
							
				break;
			
			default:
				break;
		}
	}
	else{
			J_high = 0;
	}
	
	
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		
		J_left++;
		switch(J_left){
			case 1:
							
				break;
			
			default:
				break;
		}
	}
	else{
			J_left = 0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		
		J_right++;
		switch(J_right){
			case 1:
							
				break;
			
			default:
				break;
		}
	}
	else{
			J_right = 0;
	}
	
	
	/* button management */
	if(down_0 != 0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			down_0++;				
			
			switch(down_0){
				case 2:	
					
					break;
				
				default:
					break;
			}
		}
		else {	/* button released */
			down_0 = 0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_1 != 0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			down_1++;				
			switch(down_1){
				case 2:	
					
					break;
				
				default:
					break;
			}
		}
		else {	/* button released */
			down_1 = 0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_2 != 0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			down_2++;				
			switch(down_2){
				case 2:	
					
					break;
				
				default:
					break;
			}
		}
		else {	/* button released */
			down_2 = 0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}