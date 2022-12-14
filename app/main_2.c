////////*** INCLUDES *****/////////
#include <math.h>
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
#define DELAY_PERIOD							640000


void TIMER1IntHandler(void);


int main(void)
{
		
	//	uint32_t on = 1 , off = 3 ; // in usec
	//	uint32_t period = off + on	;
		//uint32_t PWM_Off = (off/period) *400;
		//uint32_t PWM_On = (on/period) *400;
		//uint32_t delay = round((on/period) * DELAY_PERIOD); 
	
	
		IntMasterEnable(); 
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
		//while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)) //Espera a que el modulo del T0 este listo
    //{}
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC ); 
    TimerLoadSet(TIMER1_BASE, TIMER_A , DELAY_PERIOD-1);
    TimerIntRegister(TIMER1_BASE, TIMER_A, TIMER1IntHandler);
    IntEnable(INT_TIMER1A_TM4C123);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
	
	//GPIO
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
		// PIN_0 is normally locked.  Need to write a special passcode and commit it.
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
 
	
	//Configure PF1 Pin as PWM
		GPIOPinConfigure(GPIO_PF0_M1PWM4);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
		GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 ); 
		
		//Configure PWM Options
    //PWM_GEN_2 Covers M1PWM4 and M1PWM5
		PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); 
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
		// In synchronous mode, the parameter updates are not applied until a synchronization event occurs.
		// In count down mode, it counts from a value down to zero,
		//	 and then resets to the preset value, producing left-aligned PWM signals


		//Set the Period (expressed in clock ticks)
     PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 400);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 400);
		//
		// Set the pulse width of PWM0 for a 50% duty cycle.
		//
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, 300);
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,300);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,300);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,300);

     // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Turn on the Output pins
    PWMOutputState(PWM1_BASE,(PWM_OUT_4_BIT| PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT), true);
		
									// Loop forever . 
		while(1) 
		{  
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 300);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,300);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,300);
		}
}


void TIMER1IntHandler(void){
	
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 100);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,100);
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,100);
	
			TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);	/* TIMER1 timeout flag bit clears*/ 
}
