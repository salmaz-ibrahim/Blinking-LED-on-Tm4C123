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
#define DELAY_PERIOD							64000


void TIMER1IntHandler(void);


int main(void)
{
		uint32_t on = 1 , off = 4 ;
		uint32_t period = off + on	;
		uint32_t PWM_Off = (off/period) *400;
		uint32_t PWM_On = (on/period) *400;
		uint32_t off_delay = off/period * DELAY_PERIOD;
	
	
		IntMasterEnable(); 
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
		//while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)) //Espera a que el modulo del T0 este listo
    //{}
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC ); 
    TimerLoadSet(TIMER1_BASE, TIMER_A | TIMER_B, off_delay-1);
    TimerIntRegister(TIMER1_BASE, TIMER_A, TIMER1IntHandler);
    IntEnable(INT_TIMER1A_TM4C123);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT); 
    TimerEnable(TIMER1_BASE, TIMER_A);
	
	//GPIO
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
		// PIN_0 is normally locked.  Need to write a special passcode and commit it.
		GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
		GPIO_PORTF_CR_R = 0x1;
	
	// PWM
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	//Configure PF1 Pin as PWM
    GPIOPinConfigure(GPIO_PF0_M1PWM4);
		GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 );
		
		//Configure PWM Options
    //PWM_GEN_2 Covers M1PWM4 and M1PWM5
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);  
		// In synchronous mode, the parameter updates are not applied until a synchronization event occurs.
		// In count down mode, it counts from a value down to zero,
		//	 and then resets to the preset value, producing left-aligned PWM signals


		/* Set the period. For a 25-KHz frequency, the period = 1/25,000, or 40 microseconds. The PWM
			clock source is 10 MHz; the system clock divided by 2. Thus there are 400 clock ticks per period.*/
    //Set the Period (expressed in clock ticks)
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 400); //Sets the period of a PWM generator.
		//
		// Set the pulse width of PWM0 for a 25% duty cycle.
		//
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, PWM_Off);
		//
		// Set the pulse width of PWM1 for a 75% duty cycle.
		//
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, PWM_On);


    // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    // Turn on the Output pins
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT |PWM_OUT_6, true);
		
									// Loop forever . 
		while (1) 
		{  
			
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 , GPIO_PIN_1); //Turn On LED 
			
	
				
		}
}


void TIMER1IntHandler(void){
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //Turn Off LED 
	 TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);          /* TIMER1 timeout flag bit clears*/
	
	
   
}
