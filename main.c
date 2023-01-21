#include "mcc_generated_files/mcc.h"
#include "xc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "I2C_EEPROM.h"

uint16_t RXData;

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

    printf("Hello world - Testing Jamie's Board\r\n");
    
    // Uses MCC generated Read Byte function - works fine
    RXData = I2C1_Read1ByteRegister(0b01001000, 0x0F);        
    printf("Temperature Device ID (First Byte): 0x%02x \r\n", RXData);
    __delay_ms(2000);    
  
    // Uses MCC generated Read Byte function
    // Returns 0xFF which looks right, but not sure it's really working
    RXData = I2C1_Read1ByteRegister(0b1010000, 0x0020);        
    printf("EEPROM Register 0x0020 Value: 0x%02x \r\n", RXData);
    __delay_ms(2000);
    
    
    // Use EEPROM functions
    unsigned int Address = 0x0020; // Some Random Address
    unsigned char Data = 0x04;     // Some Random Data To Write
 
    /*
    EEPROM_Write(Address++, Data++); // Write 0x04 @ 0x0020
    EEPROM_Write(Address++, Data++); // Write 0x05 @ 0x0021
    EEPROM_Write(Address, Data);     // Write 0x06 @ 0x0022
    __delay_ms(10); // Wait tWR=10ms For Write To Complete
    */
    
    Address = 0x0020; // Point To First Byte Location
    
    RXData = EEPROM_Read(Address++); // Should Read 0x04
    printf("EEPROM Add: 0x%04x -  Value: 0x%02x \r\n\n", Address, RXData);   
    __delay_ms(1000);
    RXData = EEPROM_Read(Address++); // Should Read 0x05
    printf("EEPROM Add: 0x%04x -  Value: 0x%02x \r\n\n", Address, RXData);    
    __delay_ms(1000);
    RXData = EEPROM_Read(Address);   // Should Read 0x06
    printf("EEPROM Add: 0x%04x -  Value: 0x%02x \r\n\n", Address, RXData);
    __delay_ms(1000);

        
    while (1)
    {
        
        printf("Reset! \r\n");
        __delay_ms(5000);
        
    }
}
/**
 End of File
*/