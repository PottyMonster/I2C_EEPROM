/*
 * Info taken from:
 * Basic I2C Interface: 
 * https://deepbluembedded.com/i2c-communication-protocol-tutorial-pic/
 * 
 * EEPROM Operations using the above
 * https://deepbluembedded.com/reading-and-writing-serial-i2c-eeprom-with-pic/#EEPROM_Read_Operations
  
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"


void I2C1_Initialize(void);
void I2C_Wait(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Restart(void);
unsigned char I2C_Read_Byte(void);
void I2C_ACK(void);
void I2C_NACK(void);
unsigned char I2C_Write(unsigned char Data);


// #define EEPROM_Address_R 0xA1
// #define EEPROM_Address_W 0xA0

#define EEPROM_Address_R 0b10100001
#define EEPROM_Address_W 0b10100000


/*
 * Can use the MCC generated version.
void I2C1_Initialize()
{
    SSP1STAT = 0x00;
    SSP1CON1 = 0x08;
    SSP1CON2 = 0x00;
    SSP1ADD  = 0x4F;
    SSP1CON1bits.SSPEN = 0;
}

*/


void I2C_Wait()
{
    // Wait I2C Bus and Status Idle
    // Not getting out of this loop
    // SSP1CON2.SEN (Start Condition Enable) never goes low to indicate the bus is idle.
    do{
        printf("I2C Waiting Looping, SSP1STAT: 0x%02x ; SSP1CON2: 0x%02x \r\n",SSP1STAT & 0x04,SSP1CON2 & 0x1F);
        __delay_ms(500);        
    }while((SSP1STAT & 0x04) || (SSP1CON2 & 0x1F));

}
void I2C_Start()
{
  //---[ Initiate I2C Start Condition Sequence ]---
  I2C_Wait();
  SEN1 = 1;
}
void I2C_Stop()
{
  //---[ Initiate I2C Stop Condition Sequence ]---
  I2C_Wait();
  PEN1 = 1;
}
void I2C_Restart()
{
  //---[ Initiate I2C Restart Condition Sequence ]---
  I2C_Wait();
  RSEN1 = 1;
}

unsigned char I2C_Read_Byte(void)
{
  //---[ Receive & Return A Byte ]---
  I2C_Wait();
  RCEN1 = 1; // Enable & Start Reception
  while(!SSP1IF); // Wait Until Completion
  SSP1IF = 0; // Clear The Interrupt Flag Bit
  I2C_Wait();
  return SSP1BUF; // Return The Received Byte
}

void I2C_ACK(void)
{
  //---[ Send ACK - For Master Receiver Mode ]---
  I2C_Wait();
  ACKDT1 = 0; // 0 -> ACK, 1 -> NACK
  ACKEN1 = 1; // Send ACK Signal!
}
void I2C_NACK(void)
{
  //---[ Send NACK - For Master Receiver Mode ]---
  I2C_Wait();
  ACKDT1 = 1; // 1 -> NACK, 0 -> ACK
  ACKEN1 = 1; // Send NACK Signal!
}
unsigned char I2C_Write(unsigned char Data)
{
  //---[ Send Byte, Return The ACK/NACK ]---
  I2C_Wait();
  SSP1BUF = Data;
  I2C_Wait();
  return ACKSTAT1;
}




//======================================================
 
//---------------[ I2C EEPROM Routines ]----------------
//------------------------------------------------------
//-----[ Write Operations ]-----
// EEPROM Write Byte
void EEPROM_Write(unsigned int add, unsigned char data)
{
    I2C_Start();
 
    // Wait Until EEPROM Is IDLE
    while(I2C_Write(EEPROM_Address_W))
    I2C_Restart();
  
    I2C_Write(add>>8);
    I2C_Write((unsigned char)add);
    I2C_Write(data);
    I2C_Stop();
}
// EEPROM Write Page
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len)
{
    I2C_Start();
 
    // Wait Until EEPROM Is IDLE
    while(I2C_Write(EEPROM_Address_W))
    I2C_Restart();
 
    I2C_Write(add>>8);
    I2C_Write((unsigned char)add);
    for(unsigned int i=0; i<len; i++)
    I2C_Write(data[i]);
    
    I2C_Stop();
}
//-----------------------------
//-----[ Read Operations ]-----
//---[ Byte Read ]---
unsigned char EEPROM_Read(unsigned int add)
{
  unsigned char Data;
    I2C_Start();
 
  // Wait Until EEPROM Is IDLE
    while(I2C_Write(EEPROM_Address_W))
    I2C_Restart();
   
    I2C_Write(add>>8);
    I2C_Write((unsigned char)add);
    I2C_Restart();
 
    I2C_Write(EEPROM_Address_R);
    
    Data = I2C_Read_Byte();
    I2C_NACK();
    I2C_Stop();

    return Data;
}
//---[ Sequential Read ]---
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len)
{
    I2C_Start();
 
    // Wait Until EEPROM Is IDLE
    while(I2C_Write(EEPROM_Address_W))
    I2C_Restart();
 
    I2C_Write(add>>8);
    I2C_Write((unsigned char)add);
    I2C_Restart();
    I2C_Write(EEPROM_Address_R);
    for(unsigned int i=0; i<len; i++)
    {
        data[i] = I2C_Read_Byte();
        I2C_ACK();
    }
    I2C_Stop();
}