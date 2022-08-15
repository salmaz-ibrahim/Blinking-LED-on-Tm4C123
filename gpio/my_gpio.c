/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  my_gpio.c
 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "my_gpio.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
// The base addresses of all the GPIO modules.  Both the APB and AHB apertures
// are provided.
//
//*****************************************************************************
static const uint32_t g_pui32GPIOBaseAddrs[] =
{
    GPIO_PORTA_BASE, GPIO_PORTA_AHB_BASE,
    GPIO_PORTB_BASE, GPIO_PORTB_AHB_BASE,
    GPIO_PORTC_BASE, GPIO_PORTC_AHB_BASE,
    GPIO_PORTD_BASE, GPIO_PORTD_AHB_BASE,
    GPIO_PORTE_BASE, GPIO_PORTE_AHB_BASE,
    GPIO_PORTF_BASE, GPIO_PORTF_AHB_BASE,
    GPIO_PORTG_BASE, GPIO_PORTG_AHB_BASE,
    GPIO_PORTH_BASE, GPIO_PORTH_AHB_BASE,
    GPIO_PORTJ_BASE, GPIO_PORTJ_AHB_BASE,
    GPIO_PORTK_BASE, GPIO_PORTK_BASE,
    GPIO_PORTL_BASE, GPIO_PORTL_BASE,
    GPIO_PORTM_BASE, GPIO_PORTM_BASE,
    GPIO_PORTN_BASE, GPIO_PORTN_BASE,
    GPIO_PORTP_BASE, GPIO_PORTP_BASE,
    GPIO_PORTQ_BASE, GPIO_PORTQ_BASE,
    GPIO_PORTR_BASE, GPIO_PORTR_BASE,
    GPIO_PORTS_BASE, GPIO_PORTS_BASE,
    GPIO_PORTT_BASE, GPIO_PORTT_BASE,
};
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/******************************************************************************
* \Syntax          : GPIODirModeSet(uint32_t ui32Port, uint8_t ui8Pins, uint32_t ui32PinIO)   
* \Description     : Sets the direction and mode of the specified pin(s).
This function configures the specified pin(s) on the selected GPIO port as either input or output
under software control, or it configures the pin to be under hardware control.
                                                       
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) :
ui32Port is the base address of the GPIO port
ui8Pins is the bit-packed representation of the pin(s).
ui32PinIO is the pin direction and/or mode.
                  
* \Parameters (out): None                                                      
                                
*******************************************************************************/
void
GPIODirModeSet(uint32_t ui32Port, uint8_t ui8Pins, uint32_t ui32PinIO)
{
    //
    // Set the pin direction and mode.
    //
    HWREG(ui32Port + GPIO_O_DIR) = ((ui32PinIO & 1) ?
                                    (HWREG(ui32Port + GPIO_O_DIR) | ui8Pins) :
                                    (HWREG(ui32Port + GPIO_O_DIR) & ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_AFSEL) = ((ui32PinIO & 2) ?
                                      (HWREG(ui32Port + GPIO_O_AFSEL) |
                                       ui8Pins) :
                                      (HWREG(ui32Port + GPIO_O_AFSEL) &
                                       ~(ui8Pins)));
}

/******************************************************************************
* \Syntax          : GPIOPinTypeGPIOOutput(uint32_t ui32Port, uint8_t ui8Pins)        
* \Description     : Configures pin(s) for use as GPIO outputs.                                    
*                    The GPIO pins must be properly configured in order to function correctly as
					GPIO outputs.  This function provides the proper configuration for those pin(s).                                                         
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : 		ui32Port is the base address of the GPIO port.
							ui8Pins is the bit-packed representation of the pin(s).                  
* \Parameters (out): None                                                      
                                
*******************************************************************************/
void
GPIOPinTypeGPIOOutput(uint32_t ui32Port, uint8_t ui8Pins)
{
    //
    // Set the pad(s) for standard push-pull operation.
    //
    GPIOPadConfigSet(ui32Port, ui8Pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

    //
    // Make the pin(s) be outputs.
    //
    GPIODirModeSet(ui32Port, ui8Pins, GPIO_DIR_MODE_OUT);
}
/******************************************************************************
* \Syntax          : GPIOPinConfigure(uint32_t ui32PinConfig)
                 uint32_t ui32Strength, uint32_t ui32PinType)        
* \Description     : Configures the alternate function of a GPIO pin.                                    
* \Alternate : Alternate                                                                            
                                            
* \Parameters (in) : 		ui32Port is the base address of the GPIO port.
							ui8Pins is the bit-packed representation of the pin(s).
							ui32Strength specifies the output drive strength.
							ui32PinType specifies the pin type.                  
* \Parameters (out): None                                                      
                                
*******************************************************************************/
void
GPIOPinConfigure(uint32_t ui32PinConfig)
{
    uint32_t ui32Base, ui32Shift;

    //
    // Extract the base address index from the input value.
    //
    ui32Base = (ui32PinConfig >> 16) & 0xff;

    //
    // Get the base address of the GPIO module, selecting either the APB or the
    // AHB aperture as appropriate.
    //
    if(HWREG(SYSCTL_GPIOHBCTL) & (1 << ui32Base))
    {
        ui32Base = g_pui32GPIOBaseAddrs[(ui32Base << 1) + 1];
    }
    else
    {
        ui32Base = g_pui32GPIOBaseAddrs[ui32Base << 1];
    }

    //
    // Extract the shift from the input value.
    //
    ui32Shift = (ui32PinConfig >> 8) & 0xff;

    //
    // Write the requested pin muxing value for this GPIO pin.
    //
    HWREG(ui32Base + GPIO_O_PCTL) = ((HWREG(ui32Base + GPIO_O_PCTL) &
                                      ~(0xf << ui32Shift)) |
                                     ((ui32PinConfig & 0xf) << ui32Shift));
}


/******************************************************************************
* \Syntax          : GPIOPinWrite(uint32_t ui32Port, uint8_t ui8Pins, uint8_t ui8Val)      
* \Description     : Writes a value to the specified pin(s).                                    
*                                                                             
                                            
* \Parameters (in) : 		ui32Port is the base address of the GPIO port.
							ui8Pins is the bit-packed representation of the pin(s).
							ui32Strength specifies the output drive strength.
							ui8Val is the value to write to the pin(s).                  
* \Parameters (out): None                                                      
                                
*******************************************************************************/
void
GPIOPinWrite(uint32_t ui32Port, uint8_t ui8Pins, uint8_t ui8Val)
{
    //
    // Write the pins.
    //
    HWREG(ui32Port + (GPIO_O_DATA + (ui8Pins << 2))) = ui8Val;
}


//*****************************************************************************
//
//! Configures pin(s) for use by the PWM peripheral.
//!
//! \param ui32Port is the base address of the GPIO port.
//! \param ui8Pins is the bit-packed representation of the pin(s).
//!
//! The PWM pins must be properly configured for the PWM peripheral to function
//! correctly.  This function provides a typical configuration for those
//! pin(s); other configurations may work as well depending upon the board
//! setup (for example, using the on-chip pull-ups).
//!
//! The pin(s) are specified using a bit-packed byte, where each bit that is
//! set identifies the pin to be accessed, and where bit 0 of the byte
//! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
//!
//! \note This function cannot be used to turn any pin into a PWM pin; it only
//! configures a PWM pin for proper operation.  Note that a GPIOPinConfigure()
//! function call is also required to properly configure a pin for the PWM
//! function.
//!
//! \note A subset of GPIO pins on Tiva devices, notably those used by the
//! JTAG/SWD interface and any pin capable of acting as an NMI input, are
//! locked against inadvertent reconfiguration.  These pins must be unlocked
//! using direct register writes to the relevant GPIO_O_LOCK and GPIO_O_CR
//! registers before this function can be called.  Please see the ``gpio_jtag''
//! example application for the mechanism required and consult your part
//! datasheet for information on affected pins.
//!
//! \return None.
//
//*****************************************************************************
void
GPIOPinTypePWM(uint32_t ui32Port, uint8_t ui8Pins)
{
    //
    // Make the pin(s) be peripheral controlled.
    //
    GPIODirModeSet(ui32Port, ui8Pins, GPIO_DIR_MODE_HW);

    //
    // Set the pad(s) for standard push-pull operation.
    //
    GPIOPadConfigSet(ui32Port, ui8Pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
}


/*****************************************************************************
//
//! Sets the pad configuration for the specified pin(s).
//!
//! \param ui32Port is the base address of the GPIO port.
//! \param ui8Pins is the bit-packed representation of the pin(s).
//! \param ui32Strength specifies the output drive strength.
//! \param ui32PinType specifies the pin type.
//!
//! This function sets the drive strength and type for the specified pin(s)
//! on the selected GPIO port.
//*/
void
GPIOPadConfigSet(uint32_t ui32Port, uint8_t ui8Pins,
                 uint32_t ui32Strength, uint32_t ui32PinType)
{
    uint8_t ui8Bit;

    // Set the GPIO peripheral configuration register first as required.  This
    // register only appears in TM4E111 and later device classes, but is a
    // harmless write on older devices. Walk pins 0-7 and clear or set the
    // provided PC[EDMn] encoding.
    //
    for(ui8Bit = 0; ui8Bit < 8; ui8Bit++)
    {
        if(ui8Pins & (1 << ui8Bit))
        {
            HWREG(ui32Port + GPIO_O_PC) = (HWREG(ui32Port + GPIO_O_PC) &
                                           ~(0x3 << (2 * ui8Bit)));
            HWREG(ui32Port + GPIO_O_PC) |= (((ui32Strength >> 5) & 0x3) <<
                                            (2 * ui8Bit));
        }
    }

    //
    // Set the output drive strength.
    //
    HWREG(ui32Port + GPIO_O_DR2R) = ((ui32Strength & 1) ?
                                     (HWREG(ui32Port + GPIO_O_DR2R) |
                                      ui8Pins) :
                                     (HWREG(ui32Port + GPIO_O_DR2R) &
                                      ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_DR4R) = ((ui32Strength & 2) ?
                                     (HWREG(ui32Port + GPIO_O_DR4R) |
                                      ui8Pins) :
                                     (HWREG(ui32Port + GPIO_O_DR4R) &
                                      ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_DR8R) = ((ui32Strength & 4) ?
                                     (HWREG(ui32Port + GPIO_O_DR8R) |
                                      ui8Pins) :
                                     (HWREG(ui32Port + GPIO_O_DR8R) &
                                      ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_SLR) = ((ui32Strength & 8) ?
                                    (HWREG(ui32Port + GPIO_O_SLR) |
                                     ui8Pins) :
                                    (HWREG(ui32Port + GPIO_O_SLR) &
                                     ~(ui8Pins)));

    //
    // Set the 12-mA drive select register.  This register only appears in
    // TM4E111 and later device classes, but is a harmless write on older
    // devices.
    //
    HWREG(ui32Port + GPIO_O_DR12R) = ((ui32Strength & 0x10) ?
                                      (HWREG(ui32Port + GPIO_O_DR12R) |
                                       ui8Pins) :
                                      (HWREG(ui32Port + GPIO_O_DR12R) &
                                       ~(ui8Pins)));

    //
    // Set the pin type.
    //
    HWREG(ui32Port + GPIO_O_ODR) = ((ui32PinType & 1) ?
                                    (HWREG(ui32Port + GPIO_O_ODR) | ui8Pins) :
                                    (HWREG(ui32Port + GPIO_O_ODR) & ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_PUR) = ((ui32PinType & 2) ?
                                    (HWREG(ui32Port + GPIO_O_PUR) | ui8Pins) :
                                    (HWREG(ui32Port + GPIO_O_PUR) & ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_PDR) = ((ui32PinType & 4) ?
                                    (HWREG(ui32Port + GPIO_O_PDR) | ui8Pins) :
                                    (HWREG(ui32Port + GPIO_O_PDR) & ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_DEN) = ((ui32PinType & 8) ?
                                    (HWREG(ui32Port + GPIO_O_DEN) | ui8Pins) :
                                    (HWREG(ui32Port + GPIO_O_DEN) & ~(ui8Pins)));

    //
    // Set the wake pin enable register and the wake level register.  These
    // registers only appear in TM4E111 and later device classes, but are
    // harmless writes on older devices.
    //
    HWREG(ui32Port + GPIO_O_WAKELVL) = ((ui32PinType & 0x200) ?
                                        (HWREG(ui32Port + GPIO_O_WAKELVL) |
                                         ui8Pins) :
                                        (HWREG(ui32Port + GPIO_O_WAKELVL) &
                                         ~(ui8Pins)));
    HWREG(ui32Port + GPIO_O_WAKEPEN) = ((ui32PinType & 0x300) ?
                                        (HWREG(ui32Port + GPIO_O_WAKEPEN) |
                                         ui8Pins) :
                                        (HWREG(ui32Port + GPIO_O_WAKEPEN) &
                                         ~(ui8Pins)));

    //
    // Set the analog mode select register.
    //
    HWREG(ui32Port + GPIO_O_AMSEL) =
        ((ui32PinType == GPIO_PIN_TYPE_ANALOG) ?
         (HWREG(ui32Port + GPIO_O_AMSEL) | ui8Pins) :
         (HWREG(ui32Port + GPIO_O_AMSEL) & ~(ui8Pins)));
}
/**********************************************************************************************************************
 *  END OF FILE: my_gpio.c
 *********************************************************************************************************************/
