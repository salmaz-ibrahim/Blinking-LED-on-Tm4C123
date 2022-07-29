//#include "my_TM4C1234GH6PM.h" 
#include "tm4c123gh6pm.h"
#include "lm
int main() {
	//configure GPIO F blue LED (PE2)
			SYSCTL->RCGCGPIO |= (1<<5); // enable clock on PORT F
			GPIOF->AFSEL &= (1<<2); // disable alternete function on PF2
			GPIOF->DIR |= (1<<2); //make PE2 Output
			GPIOF->DEN |=(1<< 2); // digital i/o enable PE2
			GPIOF->DATA |= (1<<2); / PF1 set to HIGH (led on)
			//11.4 Initialization and Confiquration
			//To use a GPTM, the appropriate TINERn bit must be set in the RCGCTINER
			SYSCTL->RCGCTIMER |= (1<<2);
				//11.4.1 One-Shot/Feriodic Tiner Mode "Data Sheet"
			//1. Ensure the tiner is disabled (the TnE bit in the GPTMCTL register is cleared) before making
			TIMER0->CTL &= (l<<0);
			//2. rite the GPTM Configuration Register (GPTMCFG) vith a value of Ox0000.0000.
			TIMER0->CFG = Ox00000000;
			//3. Contiqure the TaNR Tield in the GPIN Tiner n Mode Register (GPNTaNR)
			TIMER0->TAMR (0x2<<0);
			/*4. optional1y confiqure the TnSNAPS, TnROT, TnNTE, and TnCDIR bits in the GPIMTnMR register
			to select vhether to capture the value of the free-running tiner at tine-out, use an externel1
	
	*/
			TIMER0->TAMR&= ~(1<<4);
			
			TIMER0-> = 0x00F42400;
	
}