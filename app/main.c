////////*** INCLUDES *****/////////

#include "C:\Users\MaNo\AppData\Local\Arm\Packs\Keil\TM4C_DFP\1.1.0\Device\Include\TM4C123\TM4C123GH6PM.h"
#include "my_TM4C123GH6PM.h"
#include "core_cm4.h"
#include "timer.h"
#include "sysctl.h"
#include "hw_timer.h"
#include "macros.h"

/////////////////////////////////////////////////

////////////DEFINE///////////////////////
#define RCGCGPIO * ((unsigned int *) 0x400FE608)
#define RCGCTIMER * ((unsigned int *) 0x400FE604)
	

#define GPIO_PORTF_CLK_EN  0x20
#define GPIO_PORTF_PIN2_EN 0x04
#define LED_ON2 	 				  0x04



#define DELAY_VALUE							300


/* First configure the timer interrupt of TM4C123 to generate an interrupt
 after delay value. Second, inside the interrupt service routine of the timer.
*/

int main(void)
{
			//
		// Enable the Timer0 peripheral

	RCGCGPIO |= (1U<<5); /*Enable clock for Port F*/
  RCGCTIMER |= (1U<<0); /*Enable clock for timer0*/

		//1. TimerDisable: 

	TimerDisable((TIMER0_BASE),TIMER_A); // TIMER_O_TAR: GPTM Timer A Mode

	//2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000 
	
	TimerConfigure((TIMER0_BASE),CLEAR); // Clear Configuration

	//3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR)
	// also config TIMER_CFG_A_ACT_TOINTD as to mask the timeout interrupt of timer A.
	
	TimerConfigure((TIMER0_BASE),TIMER_CFG_PERIODIC); 
	// Config: TIMER_CFG_ONE_SHOT_UP | TIMER_CFG_A_ACT_TOINTD
	

	//4. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).
	
	TimerLoadSet((TIMER0_BASE ),TIMER_A ,DELAY_VALUE); // Load value of timer to timer A 

	//5. If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register (GPTMIMR).
	
	TimerIntEnable((TIMER0_BASE),TIMER_TIMA_TIMEOUT); // Flags being TIMER_TIMA_TIMEOUT; i.e. timer a timeout interrupt 

	//6.  enable the timer and start counting.
	 TimerEnable((TIMER0_BASE), TIMER_A); 

	/*7.In both cases,the status flags are cleared by writing a 1 to the appropriate bit
	 of the GPTM Interrupt ClearRegister (GPTMICR). */

	GPIO_PORTF_DIR_R       |= GPIO_PORTF_PIN2_EN ; //set GREEN pin as a digital output pin
  GPIO_PORTF_DEN_R       |= GPIO_PORTF_PIN2_EN ;
	
									// Loop forever . 
		while (1) 
		{  
			
			GPIO_PORTF_DATA_R |= LED_ON2;
			
	
				
		}
}


void TIMER0A_Handler(void){
	if(TIMER_O_MIS& 0x1)
				GPIO_PORTF_DATA_R &= (LED_ON2);
				 
      TimerIntClear(TIMER0_BASE, 0x1);          /* Timer0A timeout flag bit clears*/
}

