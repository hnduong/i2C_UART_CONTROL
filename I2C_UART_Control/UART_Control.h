/*
 * UART_Control.h
 *
 *  Created on: Mar 6, 2016
 *  Author: Christian Merino
 */

#ifndef I2C_UART_CONTROL_UART_CONTROL_H_
#define I2C_UART_CONTROL_UART_CONTROL_H_
#define MRX BIT5
#define MTX BIT4
#define MODBUSCTL0 0xCA

volatile unsigned char *CO2RxData;
volatile unsigned char *CO2TxData;
volatile unsigned char CO2RXByteCtr;
volatile unsigned char CO2TXByteCtr;
volatile unsigned char Co2data[128]; // Allocate 128 bytes of data for the recieve buffer

// functions created
void registerDefinitions(void);
void clkandpinInitialization(void);
void UARTinterruptEnable(void);
void getData(void);
void sendAddress(unsigned char address);
void sendInstruction(unsigned char instruction);


#endif /* I2C_UART_CONTROL_UART_CONTROL_H_ */
