/*
 * i2c_Control.h
 *
 *  Created on: Mar 5, 2016
 *      Author: Chau
 */

#ifndef I2C_CONTROL_I2C_CONTROL_H_
#define I2C_CONTROL_I2C_CONTROL_H_
#define LIS3DH 1
#define SHT25 2

volatile unsigned char *PRxData;          // Global Pointer to RX data
volatile unsigned char PTxData;		  // Global Pointer to TX data
volatile unsigned char RXByteCtr;		  // Used for counting the RX bytes recieved
volatile unsigned char RxBuffer[128];     // Allocate 128 byte of RAM for recieving
volatile unsigned char TXByteCtr;		  // Used for Counting the TX bytes sent

//functions we created
unsigned char slaveSelect(int n); // Call this function to return the slave address we are using
void setI2C(void); // Call this function to initialize i2c conditions
void dataRead( void ); // Call this funtion to recieve data
void dataWrite (unsigned char data)	; // Call this function to write data to the microcontroller

#endif /* I2C_CONTROL_I2C_CONTROL_H_ */
