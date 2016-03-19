/*
 * UART_Control.c
 *
 *  Created on: Mar 6, 2016
 *  Author: Christian Merino
 */

#include "msp430f5229.h"
#include "UART_Control.h"

int i;

void registerDefinitions(void) {
	//UCSYNC |= 0; //ASYNCHRONOUS MODE
	WDTCTL = WDTPW + WDTHOLD; //STOP WATCHDOG TIMER

	//UCA1CTL0 REGISTER DEFINITIONS
	//UCPEN |= 1; //PARITY ENABLED (MODBUS PROTOCOL), FOR DEFAULT PROTOCOL, USE UCPEN = 0
	//UCPAR |= 1; //EVEN PARITY (MODBUS PROTOCOL)
	//UCMSB |= 0; //LSB FIRST (SENSOR DATASHEET)
	//UC7BIT |= 0; //8 BITS OF DATA
	//UCSPB |= 1; //2 STOP BITS (MODBUS PROTOCOL), 1 STOP BIT -> UCSPB = 0
	//UCMODE1 |= 3; //SELECTS UART MODE WITH AUTO BAUD-RATE DETECTION
}

void clkandpinInitialization(void) {
	//UCA1CTL1 REGISTER DEFINITIONS

	P4SEL |= MRX | MTX;
	//P4SEL2 |= MRX | MTX;

	UCA1CTL1 |= UCSSEL1 | UCTXBRK ; //SELECT UART CLOCK AS SMCLK and enables tx break
	UCA1BR0 = 3; //1000000/9600/16
	UCA1BR1 = 0; //REMAINDER OF UCA1BR0
	UCA1MCTL = 0x31; // UCBRFx (4 MSB) = 8, UCBRSx(3 BITS) = 0, UCOS16 (1 LSB) = 1 --> 1000-000-1b = 129
	UCA1CTL0 = MODBUSCTL0;

	UCA1CTL1 &= ~UCSWRST; //INITIALIZE USCI STATE MACHINE

}

void UARTinterruptEnable(void) {
	UCA1IE |= UCTXIE | UCRXIE;
}

void getData(void) {

	CO2RxData = (unsigned char *)Co2data;

	for (i = 0; i < 8; i++) {
			__bis_SR_register(GIE);
		}

}

void sendAddress(unsigned char address){

	UCA1CTL1 |= UCTXADDR; //enable send address
	unsigned char temp[] = {address};			/*adding funtion to change the non-global variable.
												 storing data into a temp array*/
	CO2TxData = (unsigned char *)temp;        /*Assigning PTxData pointer to point to the temp Array*/
	CO2TXByteCtr = sizeof temp;                /*set the counter TXbyteCtr to the size of the temp array*/


	__bis_SR_register(GIE);

}

void sendInstruction(unsigned char instruction){

	UCA1CTL1 &= ~UCTXADDR; // enable send data/instruction
	unsigned char temp[] = {instruction};			/*adding funtion to change the non-global variable.
												 storing data into a temp array*/
	CO2TxData = (unsigned char *)temp;        /*Assigning PTxData pointer to point to the temp Array*/
	CO2TXByteCtr = sizeof temp;                /*set the counter TXbyteCtr to the size of the temp array*/

	 __bis_SR_register(GIE);
}

#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
	switch(__even_in_range(UCA1IV,4))
	{
	case 0: break;
	case 2:
		UCA1IFG &= ~UCTXIFG; // reset the interrupt flag
		*CO2RxData++ = UCA1RXBUF;
		// __bic_SR_register_on_exit(LPM0_bits);
		break;
	case 4: // should jump here if there is nothing in the transmission buffer
		UCA1IFG &= ~UCRXIFG; // reset the interrupt flag

		//__delay_cycles(10);
		UCA1TXBUF = *CO2TxData;
		 //__bic_SR_register_on_exit(LPM0_bits);
		break;

	default: break;
	}
}








