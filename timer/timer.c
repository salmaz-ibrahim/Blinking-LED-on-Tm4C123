#include <stdbool.h>
#include <stdint.h>
#include "hw_memmap.h"
#include "hw_types.h"
#include "timer_types.h"
#include "timer.h"
#include "hw_timer.h"
#include "hw_sysctl.h"

#define NEW_TIMER_CONFIGURATION CLASS_IS_TM4C129

//
//! Enables the timer(s).
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Timer specifies the timer(s) to enable; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//!
//! This function enables operation of the timer module.  The timer must be
//! configured before it is enabled.
//!
//! \return None.
//
//*****************************************************************************
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
void
TimerDisable(uint32_t ui32Base, uint32_t ui32Timer)
{
    //
    // Disable the timer module.
    //
    HWREG(ui32Base + TIMER_O_CTL) &= ~(ui32Timer &
                                       (TIMER_CTL_TAEN | TIMER_CTL_TBEN));
}


//
//! Configures the timer(s).
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Config is the configuration for the timer.
//!
//! This function configures the operating mode of the timer(s).  The timer
//! module is disabled before being configured and is left in the disabled
//! state.  The timer can be configured to be a single full-width timer
//! by using the \b TIMER_CFG_* values or a pair of half-width timers using the
//! \b TIMER_CFG_A_* and \b TIMER_CFG_B_* values passed in the \e ui32Config
//! parameter.
//!
//! The configuration is specified in \e ui32Config as one of the following
//! values:
//!
//! - \b TIMER_CFG_ONE_SHOT - Full-width one-shot timer
//! - \b TIMER_CFG_ONE_SHOT_UP - Full-width one-shot timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_PERIODIC - Full-width periodic timer
//! - \b TIMER_CFG_PERIODIC_UP - Full-width periodic timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_RTC - Full-width real time clock timer
//! - \b TIMER_CFG_SPLIT_PAIR - Two half-width timers
//!
//! When configured for a pair of half-width timers, each timer is separately
//! configured.  The first timer is configured by setting \e ui32Config to
//! the result of a logical OR operation between one of the following values
//! and \e ui32Config:
//!
//! - \b TIMER_CFG_A_ONE_SHOT - Half-width one-shot timer
//! - \b TIMER_CFG_A_ONE_SHOT_UP - Half-width one-shot timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_A_PERIODIC - Half-width periodic timer
//! - \b TIMER_CFG_A_PERIODIC_UP - Half-width periodic timer that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_A_CAP_COUNT - Half-width edge count capture
//! - \b TIMER_CFG_A_CAP_COUNT_UP - Half-width edge count capture that counts
//!   up instead of down (not available on all parts)
//! - \b TIMER_CFG_A_CAP_TIME - Half-width edge time capture
//! - \b TIMER_CFG_A_CAP_TIME_UP - Half-width edge time capture that counts up
//!   instead of down (not available on all parts)
//! - \b TIMER_CFG_A_PWM - Half-width PWM output
//!
//! Some Tiva devices also allow configuring an action when the timers
//! reach their timeout.  Please consult the data sheet for the part you are
//! using to determine whether configuring actions on timers is available.
//!
//! One of the following can be combined with the  \b TIMER_CFG_* values to
//! enable an action on timer A:
//!
//! - \b TIMER_CFG_A_ACT_TOINTD - masks the timeout interrupt of timer A.
//! - \b TIMER_CFG_A_ACT_NONE - no additional action on timeout of timer A.
//! - \b TIMER_CFG_A_ACT_TOGGLE - toggle CCP on timeout of timer A.
//! - \b TIMER_CFG_A_ACT_SETTO - set CCP on timeout of timer A.
//! - \b TIMER_CFG_A_ACT_CLRTO - clear CCP on timeout of timer A.
//! - \b TIMER_CFG_A_ACT_SETTOGTO - set CCP immediately and then toggle it on
//!      timeout of timer A.
//! - \b TIMER_CFG_A_ACT_CLRTOGTO - clear CCP immediately and then toggle it on
//!      timeout of timer A.
//! - \b TIMER_CFG_A_ACT_SETCLRTO - set CCP immediately and then clear it on
//!      timeout of timer A.
//! - \b TIMER_CFG_A_ACT_CLRSETTO - clear CCP immediately and then set it on
//!      timeout of timer A.
//!
//! One of the following can be combined with the \b TIMER_CFG_* values to
//! enable an action on timer B:
//!
//! - \b TIMER_CFG_B_ACT_TOINTD - masks the timeout interrupt of timer B.
//! - \b TIMER_CFG_B_ACT_NONE - no additional action on timeout of timer B.
//! - \b TIMER_CFG_B_ACT_TOGGLE - toggle CCP on timeout of timer B.
//! - \b TIMER_CFG_B_ACT_SETTO - set CCP on timeout of timer B.
//! - \b TIMER_CFG_B_ACT_CLRTO - clear CCP on timeout of timer B.
//! - \b TIMER_CFG_B_ACT_SETTOGTO - set CCP immediately and then toggle it on
//!      timeout of timer B.
//! - \b TIMER_CFG_B_ACT_CLRTOGTO - clear CCP immediately and then toggle it on
//!      timeout of timer B.
//! - \b TIMER_CFG_B_ACT_SETCLRTO - set CCP immediately and then clear it on
//!      timeout of timer B.
//! - \b TIMER_CFG_B_ACT_CLRSETTO - clear CCP immediately and then set it on
//!      timeout of timer B.
//!
//! Similarly, the second timer is configured by setting \e ui32Config to
//! the result of a logical OR operation between one of the corresponding
//! \b TIMER_CFG_B_* values and \e ui32Config.
//!
//! \return None.
//
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

    //
    // Set the configuration of the A and B timers and set the TxPWMIE bit.
    // Note that the B timer configuration is ignored by the hardware in 32-bit
    // modes.
    //
    if(NEW_TIMER_CONFIGURATION)
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
    }
}

//
//! Controls the event type.
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Timer specifies the timer(s) to be adjusted; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//! \param ui32Event specifies the type of event; must be one of
//! \b TIMER_EVENT_POS_EDGE, \b TIMER_EVENT_NEG_EDGE, or
//! \b TIMER_EVENT_BOTH_EDGES.
//!
//! This function configures the signal edge(s) that triggers the timer when
//! in capture mode.
//!
//! \return None.
//
//*****************************************************************************
void
TimerControlEvent(uint32_t ui32Base, uint32_t ui32Timer,
                  uint32_t ui32Event)
{
    //
    // Set the event type.
    //
     ui32Timer &= TIMER_CTL_TAEVENT_M | TIMER_CTL_TBEVENT_M;
    HWREG(ui32Base + TIMER_O_CTL) = ((HWREG(ui32Base + TIMER_O_CTL) &
                                      ~ui32Timer) | (ui32Event & ui32Timer));
}

//
//! Sets the clock source for the specified timer module.
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Source is the clock source for the timer module.
//!
//! This function sets the clock source for both timer A and timer B for the
//! given timer module.  The possible clock sources are the system clock
//! (\b TIMER_CLOCK_SYSTEM) or the precision internal oscillator
//! (\b TIMER_CLOCK_PIOSC).
//!
//! \note The ability to specify the timer clock source varies with the
//! Tiva part in use.  Please consult the data sheet for the part you are
//! using to determine whether this support is available.
//!
//! \return None.
//
//*****************************************************************************
void
TimerClockSourceSet(uint32_t ui32Base, uint32_t ui32Source)
{
    //
    // Set the timer clock source.
    //
    HWREG(ui32Base + TIMER_O_CC) = ui32Source;
}

//! Sets the timer prescale value.
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Timer specifies the timer(s) to adjust; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.
//! \param ui32Value is the timer prescale value which must be between 0 and
//! 255 (inclusive) for 16/32-bit timers and between 0 and 65535 (inclusive)
//! for 32/64-bit timers.
//!
//! This function configures the value of the input clock prescaler.  The
//! prescaler is only operational when in half-width mode and is used to extend
//! the range of the half-width timer modes.  The prescaler provides the least
//! significant bits when counting down in periodic and one-shot modes; in all
//! other modes, the prescaler provides the most significant bits.
//
//
//! \return None.
//
//*****************************************************************************
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

//
//! Sets the timer load value.
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32Timer specifies the timer(s) to adjust; must be one of
//! \b TIMER_A, \b TIMER_B, or \b TIMER_BOTH.  Only \b TIMER_A should be used
//! when the timer is configured for full-width operation.
//! \param ui32Value is the load value.
//!
//! This function configures the timer load value; if the timer is running then
//! the value is immediately loaded into the timer.
//!
//! \note This function can be used for both full- and half-width modes of
//! 16/32-bit timers and for half-width modes of 32/64-bit timers.  Use
//! TimerLoadSet64() for full-width modes of 32/64-bit timers.
//!
//! \return None.
//
//*****************************************************************************
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

//
//! Clears timer interrupt sources.
//!
//! \param ui32Base is the base address of the timer module.
//! \param ui32IntFlags is a bit mask of the interrupt sources to be cleared.
//!
//! The specified timer interrupt sources are cleared, so that they no longer
//! assert.  This function must be called in the interrupt handler to keep the
//! interrupt from being triggered again immediately upon exit.
//!
//! The \e ui32IntFlags parameter has the same definition as the
//! \e ui32IntFlags parameter to TimerIntEnable().
//!
//! \note Because there is a write buffer in the Cortex-M processor, it may
//! take several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (because the interrupt controller still sees
//! the interrupt source asserted).
//!
//! \return None.
//
//*****************************************************************************
void
TimerIntClear(uint32_t ui32Base, uint32_t ui32IntFlags)
{

    //
    // Clear the requested interrupt sources.
    //
    HWREG(ui32Base + TIMER_O_ICR) = ui32IntFlags;
}
/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
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
