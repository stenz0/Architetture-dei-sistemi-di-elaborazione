#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "led/led.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

#define F1 25000000				//25Mhz
#define T1 1							//Periodo in secondi
#define F2 100000000			//100Mhz
#define T2 0.05						//50ms


int main(void)
{
  SystemInit();  													/* System Initialization (i.e., PLL)  */
	
	//Initializations
  LCD_Initialization();
	BUTTON_init();
	LED_init();
	joystick_init();
	init_RIT( T2 * F2 );											
	init_timer(0, T1 * F1 );							
	//init_timer(1, T1 * F1 );							
	//init_timer(2, T1 * F1 );						
	//init_timer(3, T1 * F1 );						
	
	
	//enables
	enable_RIT();
	//enable_timer(0);
	//enable_timer(1);
	//enable_timer(2);
	//enable_timer(3);
	//NVIC_EnableIRQ(EINT0_IRQn);
	//NVIC_EnableIRQ(EINT1_IRQn);
	//NVIC_EnableIRQ(EINT2_IRQn);
	
	LPC_SC->PCON |= 0x1;										/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
	}
}