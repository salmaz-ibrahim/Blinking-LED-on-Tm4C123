/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  my_timer.c
 *        \ Driver for the timer module.
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "my_timer.h"
#include "TM4C123.h"
/**********************************************************************************************************************
 *  LOCAL FUNCTION  
 *********************************************************************************************************************/

//*****************************************************************************
// A mapping of timer base address to interrupt number.
//*****************************************************************************
static const uint32_t g_ppui32TimerIntMap[][2] =
{
    { TIMER0_BASE, INT_TIMER0A_TM4C123 },
    { TIMER1_BASE, INT_TIMER1A_TM4C123 },
    { TIMER2_BASE, INT_TIMER2A_TM4C123 },
    { TIMER3_BASE, INT_TIMER3A_TM4C123 },
    { TIMER4_BASE, INT_TIMER4A_TM4C123 },
    { TIMER5_BASE, INT_TIMER5A_TM4C123 },
    { WTIMER0_BASE, INT_WTIMER0A_TM4C123 },
    { WTIMER1_BASE, INT_WTIMER1A_TM4C123 },
    { WTIMER2_BASE, INT_WTIMER2A_TM4C123 },
    { WTIMER3_BASE, INT_WTIMER3A_TM4C123 },
    { WTIMER4_BASE, INT_WTIMER4A_TM4C123 },
    { WTIMER5_BASE, INT_WTIMER5A_TM4C123 },
};
static const uint_fast8_t g_ui8TimerIntMapRows =
    sizeof(g_ppui32TimerIntMap) / sizeof(g_ppui32TimerIntMap[0]);


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
static uint32_t _TimerIntNumberGet(uint32_t ui32Base, uint32_t ui32Timer);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
		
//*****************************************************************************
//
//! Returns a timer modules interrupt number.
//!
//! \param ui32Base is the base address of the selected timer.
//! \param ui32Timer specifies the timer(s) to enable; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//!
//! This function returns the interrupt number for a given timer module
//! specified by the \e ui32Base and \e ui32Timer parameter.
//!
//! \return Returns a timer module's interrupt number or 0 if the interrupt
//! does not exist.
//
//*****************************************************************************
static uint32_t
_TimerIntNumberGet(uint32_t ui32Base, uint32_t ui32Timer)
{
    uint32_t ui32Int;
    uint_fast8_t ui8Idx, ui8Rows;
    const uint32_t (*ppui32SSIIntMap)[2];

    //
    // Default interrupt map.
    //
    ppui32SSIIntMap = g_ppui32TimerIntMap;
    ui8Rows = g_ui8TimerIntMapRows;

    //
    // Loop through the table that maps timer base addresses to interrupt
    // numbers.
    //
    for(ui8Idx = 0; ui8Idx < ui8Rows; ui8Idx++)
    {
        //
        // See if this base address matches.
        //
        if(ppui32SSIIntMap[ui8Idx][0] == ui32Base)
        {
            ui32Int = ppui32SSIIntMap[ui8Idx][1];

            if(ui32Timer == TIMER_B)
            {
                ui32Int += 1;
            }

            //
            // Return the corresponding interrupt number.
            //
            return(ui32Int);
        }
    }

    //
    // The base address could not be found, so return an error.
    //
    return(0);
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
 
/******************************************************************************
* \Syntax          : TimerEnable(uint32_t ui32Base, uint32_t ui32Timer)        
* \Description     :  Enables the timer(s). The timer must be
						configured before it is enabled.                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : \param ui32Base is the base address of the timer module.
					\param ui32Timer specifies the timer(s) to enable.
* \Parameters (out): None                                                      
* \Return value:   : None
*                                                                      
*******************************************************************************/

void
TimerEnable(uint32_t ui32Base, uint32_t ui32Timer)
{
    //
    // Enable the timer(s) module.
    //
    HWREG(ui32Base + TIMER_O_CTL) |= ui32Timer & (TIMER_CTL_TAEN |
                                                  TIMER_CTL_TBEN);
}
//
//! Disables the timer(s).
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Timer specifies the timer(s) to disable; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//!
//! This function disables operation of the timer module.
//!
//! \return None.
//
/******************************************************************************
* \Syntax          : TimerDisable(uint32_t ui32Base, uint32_t ui32Timer)       
* \Description     : Disables the timer(s).                                    
*                                                                                                                        
* \Parameters (in) : ui32Base is the base address of the timer module.   
					ui32Timer specifies the timer(s) to disable;
* \Parameters (out): None                                                      
* \Return value:   : None
*                                                                      
*******************************************************************************/
void
TimerDisable(uint32_t ui32Base, uint32_t ui32Timer)
{
    //
    // Disable the timer module.
    //
    HWREG(ui32Base + TIMER_O_CTL) &= ~(ui32Timer &
                                       (TIMER_CTL_TAEN | TIMER_CTL_TBEN));
}
/******************************************************************************
* \Syntax          : TimerConfigure(uint32_t ui32Base, uint32_t ui32Config)        
* \Description     : Configures the timer(s).                                    
*                                                                             
                                            
* \Parameters (in) : ui32Base is the base address of the timer module.
						ui32Config is the configuration for the timer.
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void
TimerConfigure(uint32_t ui32Base, uint32_t ui32Config)
{
    
    //
    // Disable the timers.
    //
    HWREG(ui32Base + TIMER_O_CTL) &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);

    //
    // Set the global timer configuration.
    //
    HWREG(ui32Base + TIMER_O_CFG) = ui32Config >> 24;
		if(ui32Base == TIMER1_BASE){
			TIMER1->CFG = 0x00;
			TIMER1->TAMR = 0x02;
			TIMER1->TBMR = 0x02;
		}
		if(ui32Base == TIMER0_BASE){
			TIMER0->CFG = 0x00;
			TIMER0->TAMR = 0x02;
			TIMER0->TBMR = 0x02;
		}
    //
    // Set the configuration of the A and B timers and set the TxPWMIE bit.
    // Note that the B timer configuration is ignored by the hardware in 32-bit
    // modes.
    //
    /*if(NEW_TIMER_CONFIGURATION)
    {
        HWREG(ui32Base + TIMER_O_TAMR) = (((ui32Config & 0x000f0000) >> 4) |
                                          (ui32Config & 0xff) |
                                          TIMER_TAMR_TAPWMIE);
        HWREG(ui32Base + TIMER_O_TBMR) = (((ui32Config & 0x00f00000) >> 8) |
                                          ((ui32Config >> 8) & 0xff) |
                                          TIMER_TBMR_TBPWMIE);
    }
    else
    {
        HWREG(ui32Base + TIMER_O_TAMR) = ((ui32Config & 0xff) |
                                          TIMER_TAMR_TAPWMIE);
        HWREG(ui32Base + TIMER_O_TBMR) = (((ui32Config >> 8) & 0xff) |
                                          TIMER_TBMR_TBPWMIE);
    }*/
}
/******************************************************************************
* \Syntax          : TimerPrescaleSet(uint32_t ui32Base, uint32_t ui32Timer, uint32_t ui32Value)        
* \Description     : Sets the timer prescale value.                                   
                                             
* \Parameters (in) : ui32Base is the base address of the timer module.  
					ui32Timer specifies the timer(s) to adjust
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void
TimerPrescaleSet(uint32_t ui32Base, uint32_t ui32Timer, uint32_t ui32Value)
{
    //
    // Set the timer A prescaler if requested.
    //
    if(ui32Timer & TIMER_A)
    {
        HWREG(ui32Base + TIMER_O_TAPR) = ui32Value;
    }

    //
    // Set the timer B prescaler if requested.
    //
    if(ui32Timer & TIMER_B)
    {
        HWREG(ui32Base + TIMER_O_TBPR) = ui32Value;
    }
}
/******************************************************************************
* \Syntax          : TimerLoadSet(uint32_t ui32Base, uint32_t ui32Timer, uint32_t ui32Value)        
* \Description     : Sets the timer load value.                                    
*                                                                                                                        
* \Parameters (in) : ui32Base is the base address of the timer module.
						ui32Timer specifies the timer(s) to adjust.
* \Parameters (out): None                                                      
* \Return value:   : none
                                  
*******************************************************************************/
void
TimerLoadSet(uint32_t ui32Base, uint32_t ui32Timer, uint32_t ui32Value)
{
    //
    // Set the timer A load value if requested.
    //
    if(ui32Timer & TIMER_A)
    {
        HWREG(ui32Base + TIMER_O_TAILR) = ui32Value;
    }

    //
    // Set the timer B load value if requested.
    //
    if(ui32Timer & TIMER_B)
    {
        HWREG(ui32Base + TIMER_O_TBILR) = ui32Value;
    }
}

/******************************************************************************
* \Syntax          : TimerIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags)      
* \Description     : Enables individual timer interrupt sources.                                    
                                           
* \Parameters (in) : ui32IntFlags is the bit mask of the interrupt sources to be enabled.                   
* \Parameters (out): None                                                      
* \Return value:   : none                                  
*******************************************************************************/
//! - \b TIMER_TIMB_TIMEOUT  - Timer B timeout interrupt
//! - \b TIMER_TIMA_TIMEOUT  - Timer A timeout interrupt
void
TimerIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags)
{

    //
    // Enable the specified interrupts.
    //
    HWREG(ui32Base + TIMER_O_IMR) |= ui32IntFlags;
}
/******************************************************************************
* \Syntax          : TimerIntClear(uint32_t ui32Base, uint32_t ui32IntFlags)      
* \Description     : Clears timer interrupt sources.                                   
                                           
* \Parameters (in) : ui32IntFlags is the bit mask of the interrupt sources to be enabled.                   
* \Parameters (out): None                                                      
* \Return value:   : none                                  
*******************************************************************************/
void
TimerIntClear(uint32_t ui32Base, uint32_t ui32IntFlags)
{

    //
    // Clear the requested interrupt sources.
    //
    HWREG(ui32Base + TIMER_O_ICR) = ui32IntFlags;
}
//*****************************************************************************
//
//! Registers an interrupt handler for the timer interrupt.
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Timer specifies the timer(s); must be one of \b TIMER_A,
//! \b TIMER_B, or \b TIMER_BOTH.
//! \param pfnHandler is a pointer to the function to be called when the timer
//! interrupt occurs.
//!
//! This function registers the handler to be called when a timer interrupt
//! occurs.  In addition, this function enables the global interrupt in the
//! interrupt controller; specific timer interrupts must be enabled via
//! TimerIntEnable().  It is the interrupt handler's responsibility to clear
//! the interrupt source via TimerIntClear().
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
TimerIntRegister(uint32_t ui32Base, uint32_t ui32Timer,
                 void (*pfnHandler)(void))
{
    uint32_t ui32Int;

    //
    // Get the interrupt number for this timer module.
    //
    ui32Int = _TimerIntNumberGet(ui32Base, ui32Timer);


    //
    // Register the interrupt handler.
    //
    IntRegister(ui32Int, pfnHandler);

    //
    // Enable the interrupt.
    //
    IntEnable(ui32Int);
}



/**********************************************************************************************************************
 *  END OF FILE: my_timer.c
 *********************************************************************************************************************/
