/*
 * Team MDMT, SIDS Monitor Project
 * Author: Chau Z Tran
 *
 * Date Created: 2/28/16
 * Modified: 3/5/26 , made the code modular, Chau Tran.
 *
 * Purpose: Test the functionality of the accerometer and other i2c devices on the board.
 */

#include <msp430f5229.h>
#include "UART_Control.h"
#include "i2c_Control.h"
#include "LIS3DH.h"
#include "SHT25.h"

// include more things here
unsigned char slaveAdrCheck;

void main (void){

	setI2C(); 			 // initialze the i2c conditions

	/*insert initializations needed for other sensors and bluetooth here*/

	P1DIR |= BIT2;       // P1.0 set as output, just want to turn on led to show that board is powered
	P1OUT = 0x00;       // using the xor operation to turn on the led pin, leave the led on.

	//UCB0I2CSA |= slaveSelect(SHT25); // select the slave, values defined in i2c_Control.h
	UCB0I2CSA |= slaveSelect(LIS3DH);

	dataWrite(WHO_AM_I);     // send command to return slave address, values defined in header files for the devices

	while(1){
			if(UCB0I2CSA == 0x18 ){ // if we are talking to the accelerometer
				RXByteCtr = 1; 		// we only need to send 1 byte of data for interpreting.
			}
			else{					// otherwise
				RXByteCtr = 2; 		// we need to send 2 bytes of data for calculating temperature.
			}
			dataRead();          	// continuously read data

			// call bluetooth function to send data once the array is full
			// rinse and repeat
	}
}

