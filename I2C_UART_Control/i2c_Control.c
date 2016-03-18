/*
 * i2c_Control.c
 *
 *  Created on: Mar 5, 2016
 *      Author: Chau
 */

#include <msp430f5229.h>
#include "i2c_Control.h"


unsigned char slaveSelect(int n) // Returns Slave Address Specified
{
	unsigned char returnValue;
	unsigned char sensor[] = {0x18, 0x40}; //slave addresses for devices, refer to data sheets

	switch (n){
	case LIS3DH:
		returnValue = sensor[0];
		break;
	case SHT25:
		returnValue =  sensor[1];
		break;
	default :
		break;
	}

	return returnValue;

}

void setI2C(void)						    // Call this function, to set up I2C
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT (Watchdog Timer)
  P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0 (for SDA pin 3.0 and SCL pin 3.1)
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 | UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 3;                             // fSCL = SMCLK/12 = ~100kHz // set to 3 for 400kh speed
  UCB0BR1 = 0;								// part of the frequency, make this zero
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0IE |= UCTXIE+UCRXIE;       		 // Enable TX interrupt and RX interrupt, monitor for NACK
}

void dataRead ( void )
{
	PRxData = (unsigned char *)RxBuffer;    // Start of RX buffer
	UCB0CTL1 &= 0xEF; 						// I2C RX enable

	while(RXByteCtr){ 						//Repeat for number of bytes needed to be recieved.
		UCB0CTL1 |= UCTXSTT;				//Enable Start Condition
		__bis_SR_register(GIE);     		//Enable Interrupts
		 __no_operation();				    /*no operation to ensure that all data has been sent*/
	}
    while (UCB0CTL1 & UCTXSTP);         // 	Ensure the stop signal got sent
}

void dataWrite (unsigned char data)			/*Data write function, data is 8 bits*/
{
    PTxData = data;        /*Assigning PTxData pointer to point to the temp Array*/

    TXByteCtr = 1;                /*set the counter TXbyteCtr to the size of the temp array*/
    UCB0CTL1 |= UCTR + UCTXSTT;		        /*Start Condition*/

    __bis_SR_register(GIE);			    /*enable interrupts*/
    __no_operation();				    /*no operation to ensure that all data has been sent*/
    while(UCB0CTL1 & UCTXSTP);		    /*ensure that the stop condition got sent*/

}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(__even_in_range(UCB0IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6: break;
  case  8:break;
  case 10:                                  // Vector 10: RXIFG
    RXByteCtr--;                            // Decrement RX byte counter
    if (RXByteCtr)
    {
      *PRxData++ = UCB0RXBUF;               // Move RX data to address PRxData
      if (RXByteCtr == 1)                   // Only one byte left?
        UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
    }
    else
    {
      *PRxData = UCB0RXBUF;                 // Move final RX data to PRxData
     // UCB0IFG &= ~UCRXIFG;					// Clear the RX flag
      __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
    }
    break;

  case 12:   								// Vector 12: TXIFG
	if (TXByteCtr)                          // Check TX byte counter
	{
	  UCB0TXBUF = PTxData;                   // Load TX buffer
	  TXByteCtr--;                          // Decrement TX byte counter
	}
	else
	{
	  UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
	  //UCB0IFG &= ~UCTXIFG;                // Clear USCI_B0 TX int flag
	  __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
	}

  default: break;
  }
}




