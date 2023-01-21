/* 
 * File:   I2C_EEPROM.h
 * Author: daniel.twining
 *
 * Created on January 20, 2023, 11:46 AM
 */

#ifndef I2C_EEPROM_H
#define	I2C_EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

    
//-----------[ Functions' Prototypes ]--------------
 
//---[ I2C Routines ]---
// void I2C1_Initialize(void);
void I2C_Wait(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Restart(void);
unsigned char I2C_Read_Byte(void);
void I2C_ACK(void);
void I2C_NACK(void);
unsigned char I2C_Write(unsigned char);


//---[ EEPROM Routines ]---
void EEPROM_Write(unsigned int add, unsigned char data);
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len);
unsigned char EEPROM_Read(unsigned int add);
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len);



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_EEPROM_H */

