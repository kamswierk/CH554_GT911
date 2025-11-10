#ifndef CORE_INC_GT911_IF_H_
#define CORE_INC_GT911_IF_H_

uint8_t GT911_ReadFromI2C(uint8_t DeviceAddress, uint16_t RegisterAddress, uint8_t *Result, uint8_t Size);
uint8_t GT911_WriteToI2C(uint8_t DeviceAddress, uint16_t RegisterAddress, uint8_t *Value, uint8_t Size);

#endif /* CORE_INC_GT911_IF_H_ */
