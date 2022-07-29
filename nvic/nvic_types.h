#include <stdint.h>


#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define NVIC_BASE           (SCS_BASE +  0x0100UL)					 	 /*!< NVIC Base Address */
#define NVIC                ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct */
 


// Vector Table IRQn Numbers 
typedef enum 
{ //exception enumeration names for IRQn values
  NonMaskableInt_IRQn = -14,
  HardFault_IRQn = -13,
  MemoryManagement_IRQn = -12,
  BusFault_IRQn = -11,
  UsageFault_IRQn = -10,
  SecureFault_IRQn = -9,
  SVCall_IRQn = -5,
  DebugMonitor_IRQn = -4,
  PendSV_IRQn = -2,
  SysTick_IRQn = -1,
  WWDG_STM_IRQn = 0,
  PVD_STM_IRQn = 1,

	/* Cortex-M4 Processor interrupts Numbers */ 
	TIMER1A_Handler = 21,	// Timer 1 subtimer A
	GPIOF_Handler  =  30 //  GPIO Port F
}IRQn_Type;