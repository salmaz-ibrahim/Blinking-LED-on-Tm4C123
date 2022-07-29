/* First configure the timer interrupt of TM4C123 to generate an interrupt
 after every one second. Second, inside the interrupt service routine of the timer, 
sample the analog signal value with ADC and turn off ADC sampling before 
returning from the interrupt service routine. */ 


#include "C:\Users\MaNo\AppData\Local\Arm\Packs\Keil\TM4C_DFP\1.1.0\Device\Include\TM4C123\TM4C123GH6PM.h"
#include "my_TM4C123GH6PM.h"
#include "core_cm4.h"
#include "timer.h"
#include "sysctl.h"


#define GPIO_PORTF_CLK_EN  0x20
#define GPIO_PORTF_PIN1_EN 0x02
#define LED_ON1 	   0x02
#define GPIO_PORTF_PIN2_EN 0x04
#define LED_ON2		   0x04
#define GPIO_PORTF_PIN3_EN 0x08
#define LED_ON3 	   0x08



#define TIMER1_TAMR_R           (*((volatile unsigned long *)0x40031004))
#define Periodic_Mode (1<<2)					//for Periodic mode.
#define OneShot_Mode 0x1					//for One Shot mode.

int main(void)
{
	SYSCTL_RCGCGPIO_R  |= 0x20; // turn on bus clock for GPIOF
  GPIO_PORTF_DIR_R       |= GPIO_PORTF_PIN1_EN ; //set GREEN pin as a digital output pin
  GPIO_PORTF_DEN_R       |= GPIO_PORTF_PIN1_EN ;
	GPIO_PORTF_DATA_R |= LED_ON1;
	//
	// Enable the Timer0 peripheral
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
		//
		// Wait for the Timer0 module to be ready.
		//
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
		{
		}
		
		//Configure TimerA as a half-width periodic timer
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_ONE_SHOT);
		
		// Set the count time for the the one-shot timer (TimerA).
	TimerLoadSet(TIMER0_BASE, TIMER_A, 3000);
		
		//Enable the timers.
	TimerEnable(TIMER0_BASE, TIMER_BOTH);

	}
 

void TIMER0A_Handler(void);
void TIMER0A_Handler(void){
	if(TIMER1->MIS & 0x1)
				GPIO_PORTF_DATA_R &= LED_ON1;
				GPIO_PORTF_DATA_R &= LED_ON2;
				GPIO_PORTF_DATA_R &= LED_ON3; 
      // TIMER1->ICR = 0x1;          /* Timer1A timeout flag bit clears*/
}
