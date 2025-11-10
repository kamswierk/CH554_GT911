#include <stdint.h>
#include "gt911_if.h"
#include "i2c.h"

uint8_t GT911_ReadFromI2C(uint8_t DeviceAddress, uint16_t RegisterAddress, uint8_t *Result, uint8_t Size)
{
	uint8_t c[2];

	c[0] = (uint8_t)(RegisterAddress >> 8);             // extract high byte of register address
	c[1] = (uint8_t)((uint16_t)(RegisterAddress));

	I2C_Write(DeviceAddress, c, sizeof(c));       		// write the RegisterAddres
	I2C_Read(DeviceAddress, Result, Size);

	return 0;
}


uint8_t GT911_WriteToI2C(uint8_t DeviceAddress, uint16_t RegisterAddress, uint8_t *Value, uint8_t Size)
{
	uint8_t c[20];										// Temporary Write buffer set to 20 TBD
	uint8_t i;

	c[0] = (uint8_t)(RegisterAddress >> 8);             // extract high byte of register address
	c[1] = (uint8_t)((uint16_t)(RegisterAddress));

	for(i = 0; i < Size; i++)
	{
		c[2 + i] = *Value;
		Value++;
	}

	I2C_Write(DeviceAddress, c, (2 + Size));			// Send 2 bytes of address and data size

	return 0;
}
