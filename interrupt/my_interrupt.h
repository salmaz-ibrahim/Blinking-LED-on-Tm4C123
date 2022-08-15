/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  <Write File Name>
 *       Module:  -
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef _MY_INTERRUPT_H
#define _MY_INTERRUPT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "my_hw_ints.h"
#include "hw_nvic.h"
#include "hw_types.h"
#include "debug.h"
#include "cpu.h"

extern bool IntMasterEnable(void);
extern void IntEnable(uint32_t ui32Interrupt);
extern void IntDisable(uint32_t ui32Interrupt);
extern void IntRegister(uint32_t ui32Interrupt, void (*pfnHandler)(void));
extern void IntUnregister(uint32_t ui32Interrupt);
#endif