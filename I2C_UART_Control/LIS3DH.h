/*
 * LIS3DH.h
 * Created on: Mar 5, 2016
 * Author: Chau
 *
 * Purpose:
 * The Accelemeter has many commands.
 * Defining  values here to make code more readable.
 * These values obtained from datasheet from this link:
 *
 */

#ifndef I2C_UART_CONTROL_LIS3DH_H_
#define I2C_UART_CONTROL_LIS3DH_H_

//commands defined below
#define WHO_AM_I 0x0F
#define OUT_Z_L 0x2C // command to return the acceleration in the z direction.
#define OUT_X_L 0x28 // x direction
#define OUT_Y_L 0x2A // y direction


#endif /* I2C_UART_CONTROL_LIS3DH_H_ */
