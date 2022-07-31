//1. TimerDisable: 
void
TimerDisable(uint32_t ui32Base,uint32_t ui32Timer);

//2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000 
void
TimerConfigure(uint32_t ui32Base,
uint32_t ui32Config); // Clear Configuration

//3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR)
// also config TIMER_CFG_A_ACT_TOINTD as to mask the timeout interrupt of timer A.
void
TimerConfigure(uint32_t ui32Base,uint32_t ui32Config); // Config: TIMER_CFG_ONE_SHOT_UP | TIMER_CFG_A_ACT_TOINTD

//4. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).
void
TimerLoadSet(uint32_t ui32Base,uint32_t ui32Timer,uint32_t ui32Value); // Load value of timer to timer A 

//5. If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register (GPTMIMR).
void
TimerIntEnable(uint32_t ui32Base,uint32_t ui32IntFlags) // Flags being TIMER_TIMA_TIMEOUT; i.e. timer a timeout interrupt 

//6.  enable the timer and start counting.
void TimerEnable (uint32_t ui32Base, uint32_t ui32Timer); 

/*7.In both cases,the status flags are cleared by writing a 1 to the appropriate bit
 of the GPTM Interrupt ClearRegister (GPTMICR). */
 


