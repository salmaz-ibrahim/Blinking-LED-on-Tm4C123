/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  my_timer.h
 *       Module:  Timer
 *
 *  Description:   Prototypes for the timer module
 *  
 *********************************************************************************************************************/
#ifndef _MY_TIMER_H_
#define _MY_TIMER_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include "hw_memmap.h"
#include "hw_types.h"
#include "timer_types.h"
#include "hw_timer.h"
#include "hw_sysctl.h"
#include "hw_ints.h"
#include "interrupt.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
 #define NEW_TIMER_CONFIGURATION CLASS_IS_TM4C129
 /*****************************************************************************
* Values that can be passed to TimerConfigure as the ui32Config parameter.
*****************************************************************************/

#define TIMER_CFG_PERIODIC       0x00000022  // Full-width periodic timer
#define TIMER_CFG_A_PERIODIC     0x00000022  // Timer A periodic timer
#define TIMER_CFG_B_PERIODIC     0x00002200  // Timer B periodic timer

/*****************************************************************************
Values that can be passed to most of the timer APIs as the ui32Timer
 parameter.
*****************************************************************************/
#define TIMER_A                 0x000000ff  // Timer A
#define TIMER_B                 0x0000ff00  // Timer B
#define TIMER_BOTH              0x0000ffff  // Timer Both

/*****************************************************************************
 Values that can be passed to TimerIntEnable, TimerIntDisable, and
 TimerIntClear as the ui32IntFlags parameter, and returned from
 TimerIntStatus.
*****************************************************************************/
#define TIMER_TIMB_DMA          0x00002000 // TimerB DMA Complete Interrupt.
#define TIMER_TIMB_MATCH        0x00000800  // TimerB match interrupt
#define TIMER_CAPB_EVENT        0x00000400  // CaptureB event interrupt
#define TIMER_CAPB_MATCH        0x00000200  // CaptureB match interrupt
#define TIMER_TIMB_TIMEOUT      0x00000100  // TimerB time out interrupt
#define TIMER_TIMA_DMA          0x00000020 // TimerA DMA Complete Interrupt.
#define TIMER_TIMA_MATCH        0x00000010  // TimerA match interrupt
#define TIMER_RTC_MATCH         0x00000008  // RTC interrupt mask
#define TIMER_CAPA_EVENT        0x00000004  // CaptureA event interrupt
#define TIMER_CAPA_MATCH        0x00000002  // CaptureA match interrupt
#define TIMER_TIMA_TIMEOUT      0x00000001  // TimerA time out interrupt

/*****************************************************************************

 Values that can be passed to TimerClockSourceSet() or returned from
 TimerClockSourceGet().

*****************************************************************************/
#define TIMER_CLOCK_SYSTEM      0x00000000
#define TIMER_CLOCK_PIOSC       0x00000001
 
 
 //*****************************************************************************
//
// These values for TimerConfigure have been deprecated.
//
//*****************************************************************************
#ifndef DEPRECATED
#define TIMER_CFG_32_BIT_OS      0x00000021  // 32-bit one-shot timer
#define TIMER_CFG_32_BIT_OS_UP   0x00000031  // 32-bit one-shot up-count timer
#define TIMER_CFG_32_BIT_PER     0x00000022  // 32-bit periodic timer
#define TIMER_CFG_32_BIT_PER_UP  0x00000032  // 32-bit periodic up-count timer
#define TIMER_CFG_32_RTC         0x01000000  // 32-bit RTC timer
#define TIMER_CFG_16_BIT_PAIR    0x04000000  // Two 16-bit timers
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
extern void TimerEnable(uint32_t ui32Base, uint32_t ui32Timer);
extern void TimerDisable(uint32_t ui32Base, uint32_t ui32Timer);
extern void TimerConfigure(uint32_t ui32Base, uint32_t ui32Config);
extern void TimerIntClear(uint32_t ui32Base, uint32_t ui32IntFlags);
extern void TimerIntEnable(uint32_t ui32Base, uint32_t ui32IntFlags);
extern void TimerPrescaleSet(uint32_t ui32Base, uint32_t ui32Timer,uint32_t ui32Value);
extern void TimerLoadSet(uint32_t ui32Base, uint32_t ui32Timer, uint32_t ui32Value);
extern void TimerIntRegister(uint32_t ui32Base, uint32_t ui32Timer,void (*pfnHandler)(void));
 
#endif  /* MY_TIMER_H */

/**********************************************************************************************************************
 *  END OF FILE: my_timer.h
 *********************************************************************************************************************/
 