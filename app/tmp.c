////////*** INCLUDES *****/////////
#include "TM4C123GH6PM.h" //Register Definitions
/////////////////////////////////////////////////

////////////DEFINE///////////////////////

#define DELAY_VALUE							1000000
#define GPIO_PORTF_CLK_EN 					 0x20
#define GPIO_PORTF_PIN1_EN 					0x02
 


void Delay(void){
	for (unsigned long j=0; j<DELAY_VALUE ; j++);
}

int main(void){
	
	// Enable the clock to the port by setting the appropriate bits in the RCGCGPIO register.
	// The bit 0 to bit 5 of RCGC_GPIO_R register are used to enable the port A to port F on the peripherals respectively.
		volatile unsigned long ulLoop ;
		SYSCTL_RCGCGPIO_R |= GPIO_PORTF_CLK_EN ;
		ulLoop = SYSCTL_RCGCGPIO_R;		
		
			
		//Setting PORTF Direction
		GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN1_EN ;		
		 //Enable clock for PORTF  // Configure ORTF Pin1, 2 and 3 digital output pins
		
		//GPIODEN register is used to enable GPIO pins as digital input-output pins.	
		
		GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN1_EN ; // Enable PORTF Pin1, 2 and 3 as a digital pins
		
		
		
		while(1) {
			GPIO_PORTF_DATA_R |= 0x04; // turn on blue LED,,,,, 0x02 RED, 0x04 BLUE, 0x08 GREEN
			Delay();
			GPIO_PORTF_DATA_R &= 0x04; //// turn Off blue LED
	}
	
}