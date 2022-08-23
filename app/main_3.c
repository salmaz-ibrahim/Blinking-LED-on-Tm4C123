////////*** INCLUDES *****/////////
#include "C:\Users\MaNo\AppData\Local\Arm\Packs\Keil\TM4C_DFP\1.1.0\Device\Include\TM4C123\TM4C123GH6PM.h"
#include "core_cm4.h"
#include "my_gpio.h"
#include "my_timer.h"
#include "my_interrupt.h"
#include "sysctl.h"
#include "hw_memmap.h"
#include "pwm.h"
#include "pin_map.h"

/////////////////////////////////////////////////

////////////DEFINE///////////////////////
#define DELAY_PERIOD_A							64000
#define DELAY_PERIOD_B							64000
#define PRESCALE_VALUE 							250

void TIMER1A_IntHandler(void);
void TIMER0A_IntHandler(void);


int main(void)
{
		IntMasterEnable(); 
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
		//while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)) 
    //{}
	
	//// For Timer 1
	// Periodic 32 bit down 
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC ); 
    TimerLoadSet(TIMER1_BASE, TIMER_A , DELAY_PERIOD_A-1);
		TimerPrescaleSet(TIMER1_BASE,TIMER_A,PRESCALE_VALUE -1 );
    TimerIntRegister(TIMER1_BASE, TIMER_A, TIMER1A_IntHandler);
    IntEnable(INT_TIMER1A_TM4C123);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
		
	
	//// For Timer 0
		// Periodic 32 bit down 
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC );
		TimerLoadSet(TIMER0_BASE, TIMER_A , DELAY_PERIOD_B-1);
		TimerPrescaleSet(TIMER0_BASE,TIMER_A,PRESCALE_VALUE -1 );
    TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0A_IntHandler);
    IntEnable(INT_TIMER0A_TM4C123);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	
	
    TimerEnable(TIMER1_BASE, TIMER_A);
		
	
	//GPIO
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		
		// Configure GPIOF pin 1 as output pin 
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
		// PIN_0 is normally locked.  Need to write a special passcode and commit it.
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
 
					// Loop until timer interrupt. 
		while(1) 
		{  
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
		}
}


void TIMER1A_IntHandler(void){
	
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0x00);			
			TimerEnable(TIMER0_BASE, TIMER_A);
			TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);	/* TIMER1 timeout flag bit clears*/ 
			TimerDisable(TIMER1_BASE, TIMER_A);
			
}

void TIMER0A_IntHandler(void){
			
			TimerEnable(TIMER1_BASE, TIMER_A);
			TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);	/* TIMER0 timeout flag bit clears*/ 
			TimerDisable(TIMER0_BASE, TIMER_A);
			
}
