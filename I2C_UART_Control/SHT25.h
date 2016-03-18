/*
 * SHT25.h
 *
 *  Created on: Mar 5, 2016
 *  Author: Chau Tran
 *
 *  Purpose:
 *  The temperature/humidity sensor has multiple commands.
 *  To make the code more readable, will define some values for use.
 *
 */

#ifndef I2C_UART_CONTROL_SHT25_H_
#define I2C_UART_CONTROL_SHT25_H_

//defining values
#define tempREAD 0xE3
#define humidREAD 0xE5
#define NHMtempREAD 0xF3
#define NHMhumidREAD 0xF5
#define writeUSER 0xE6
#define readUSER 0xE7
#define softRESET 0xFE

#endif /* I2C_UART_CONTROL_SHT25_H_ */
