#include <stdint.h>
#include "nvic_types.h"
 
void NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
uint32_t NVIC_GetPriorityGrouping(void);
void NVIC_EnableIRQ(IRQn_Type IRQn); 
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
