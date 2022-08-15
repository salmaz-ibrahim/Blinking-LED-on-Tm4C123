////////*** INCLUDES *****/////////

#include "C:\Users\MaNo\AppData\Local\Arm\Packs\Keil\TM4C_DFP\1.1.0\Device\Include\TM4C123\TM4C123GH6PM.h"
#include "core_cm4.h"
#include "my_gpio.h"
#include "my_timer.h"
#include "sysctl.h"
#include "macros.h"
#include "my_interrupt.h"
#include "C:\Users\MaNo\Documents\GitHub\Egyfwd\inc\hw_memmap.h"

/////////////////////////////////////////////////

////////////DEFINE///////////////////////
#define RCGCTIMER * ((unsigned int *) 0x400FE604)

#define DELAY_VALUE							20000000

extern void TIMER0_Handler(void);


int main(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); /*Enable clock for Port F*/
	//
	// Wait for the GPIOA module to be ready.
	//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
	{}
	// enable GPIO pins as digital I/Os
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);
	// PIN_0 is normally locked.  Need to write a special passcode and commit it.
		GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
		GPIO_PORTF_CR_R = 0x1;
	
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);


  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); /*Enable clock for timer0*/
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
	{ }

		//1. TimerDisable: 

	TimerDisable((TIMER0_BASE),TIMER_A);

	//2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000 
	
	TimerConfigure((TIMER0_BASE),0x00); // Clear Configuration

	//3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR)
	// also config TIMER_CFG_A_ACT_TOINTD as to mask the timeout interrupt of timer A.
	//TIMER_CFG_32_BIT_TIMER
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	//4. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).
	
	TimerLoadSet(TIMER0_BASE,TIMER_A ,DELAY_VALUE); // Load value of timer to timer A 
	 // Only mode available is 32bit periodic down-count

    // Set CallBack for Timer0 Interrupt
    //
  TimerIntRegister(TIMER0_BASE , TIMER_A , TIMER0_Handler);
	

	//5. If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register (GPTMIMR).
	IntEnable(INT_TIMER0A_TM4C123);
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT); // Flags being TIMER_TIMA_TIMEOUT; i.e. timer a timeout interrupt 

	//6.  enable the timer and start counting.
	 TimerEnable(TIMER0_BASE,TIMER_A); 

	/*7.In both cases,the status flags are cleared by writing a 1 to the appropriate bit
	 of the GPTM Interrupt ClearRegister (GPTMICR). */

	
									// Loop forever . 
		while (1) 
		{  
			
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , GPIO_PIN_1); //Turn On LED 
			
	
				
		}
}

void TIMER0_Handler(void){
	 TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);          /* Timer0 timeout flag bit clears*/
	if(TIMER_O_MIS & 0x1){
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //Turn Off LED 
	}
				 
     
}
