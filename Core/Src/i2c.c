#include <stdint.h>
#include "i2c.h"
#include "delay.h"
#include "ch554.h"


void I2C_Init(void)
{
	SDA_HIGH();
	SCL_HIGH();
}


static void _I2C_Delay(void)
{
#if 1
	mDelayuS(1); /* 5 : about 100kHz */
#else
	int i = 1;
	while (i--)
    ;
#endif
}


static uint8_t _I2C_ByteOut(uint8_t Byte)
{
	uint8_t i = 0;

	for (i = 0; i < 8; i++)
	{
		(Byte & 0x80)? SDA_HIGH(): SDA_LOW();
		SCL_HIGH();
		_I2C_Delay();
		SCL_LOW();
		Byte <<= 1;
		_I2C_Delay();
	}

	/* read ACK/NACK */
	SDA_HIGH(); /* pull up SDA for read */
	SCL_HIGH();
	_I2C_Delay();
	i = SDA_READ(); /* read ACK/NACK */

	SCL_LOW();
	SDA_LOW();
	_I2C_Delay();

	return i? 0: 1; /* 1:ACK, 0:NACK */
}


static uint8_t _I2C_ByteIn(uint8_t last)
{
	uint8_t i = 0;
	uint8_t b = 0;

	SDA_HIGH(); /* pull up for input */

	for (i = 0; i < 8; i++)
	{
		_I2C_Delay();
		SCL_HIGH();
		b <<= 1;
		if (SDA_READ()) b |= 1;
		_I2C_Delay();
		SCL_LOW();
	}
	_I2C_Delay();
	last? SDA_HIGH(): SDA_LOW(); /* send NACK or ACK */
	SCL_HIGH();
	_I2C_Delay();
	SCL_LOW();
	SDA_LOW();
	_I2C_Delay();

	return b;
}


static void I2C_End(void)
{
	/* stop condition */
	// SDA_LOW();
	// _I2C_Delay();
	SCL_HIGH();
	_I2C_Delay();
	SDA_HIGH();
	_I2C_Delay();
}


/* start condition, send Device + Read/Write bit */
static uint8_t _I2C_Begin(uint8_t DeviceAddress, uint8_t rw)
{
	/* start condition */
	SDA_LOW();
	_I2C_Delay();
	SCL_LOW();
	_I2C_Delay();

	/* DeviceAddress + Read/Write bit */
	DeviceAddress <<= 1;	/* shift DeviceAddress */

	if (rw)
    {
		DeviceAddress++; /* Read/Write */
    }

	/* send */
	return _I2C_ByteOut(DeviceAddress);
	}


uint8_t I2C_Read(uint8_t DeviceAddress, uint8_t *Data, uint8_t Size)
{
	uint8_t rc = _I2C_Begin(DeviceAddress, 1); /* start, set DeviceAddress with READ bit */

	if (rc)
	{
		while (Size--)
		{
			*Data++ = _I2C_ByteIn(Size == 0);
		}
	}
	I2C_End();

	return rc;
}


uint8_t I2C_Write(uint8_t DeviceAddress, uint8_t *Data, uint8_t Size)
{
	uint8_t b;
	uint8_t n = Size;
	uint8_t rc = _I2C_Begin(DeviceAddress, 0);	/* start, set DeviceAddress with WRITE bit */

	if (rc)
	{
		while (n && rc)
		{
			b = *Data;
			rc = _I2C_ByteOut(b); // 0:ack, 1:nack
			if (rc)
			{
				n--;
				Data++;
			}
		}
    rc = rc? ((Size - n)? 1: 0): 0;
	}
	I2C_End();

	return rc;
}
