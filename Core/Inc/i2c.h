/*
 * i2c.h
 *
 *  Created on: 18 mar 2024
 *      Author: DELLPC
 */

#ifndef CORE_INC_I2C_H_
#define CORE_INC_I2C_H_


#define I2C_SDAT  	MOSI
#define I2C_SCLK  	SCS

#define SDA_HIGH()  (I2C_SDAT = 1)
#define SDA_LOW() 	(I2C_SDAT = 0)
#define SDA_READ()  (I2C_SDAT)

#define SCL_HIGH()  (I2C_SCLK = 1)
#define SCL_LOW() 	(I2C_SCLK = 0)


void I2C_Init(void);
uint8_t I2C_Write(uint8_t DeviceAddress, uint8_t *Data, uint8_t Size);
uint8_t I2C_Read(uint8_t DeviceAddress, uint8_t *Data, uint8_t Size);

#endif /* CORE_INC_I2C_H_ */
