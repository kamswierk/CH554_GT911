#ifndef CORE_INC_GT911_H_
#define CORE_INC_GT911_H_

#include "gt911_structs.h"

#define GOODIX_OK   0

// 0x28/0x29 (0x14 7bit)
#define GOODIX_I2C_ADDR_28  0x14
// 0xBA/0xBB (0x5D 7bit)
#define GOODIX_I2C_ADDR_BA  0x5D

#define GOODIX_MAX_HEIGHT   4096
#define GOODIX_MAX_WIDTH    4096
#define GOODIX_INT_TRIGGER    1
#define GOODIX_CONTACT_SIZE   8
#define GOODIX_MAX_CONTACTS   10

#define GOODIX_CONFIG_MAX_LENGTH  240
#define GOODIX_CONFIG_911_LENGTH  186
#define GOODIX_CONFIG_967_LENGTH  228


/* Commands for REG_COMMAND */
//0: read coordinate state
#define GOODIX_CMD_READ         0x00
// 1: difference value original value
#define GOODIX_CMD_DIFFVAL      0x01
// 2: software reset
#define GOODIX_CMD_SOFTRESET    0x02
// 3: Baseline update
#define GOODIX_CMD_BASEUPDATE   0x03
// 4: Benchmark calibration
#define GOODIX_CMD_CALIBRATE    0x04
// 5: Off screen (send other invalid)
#define GOODIX_CMD_SCREEN_OFF   0x05

/* When data needs to be sent, the host sends command 0x21 to GT9x,
 * enabling GT911 to enter "Approach mode" and work as a transmitting terminal */
#define GOODIX_CMD_HOTKNOT_TX   0x21

#define RESOLUTION_LOC    1
#define MAX_CONTACTS_LOC  5
#define TRIGGER_LOC 6

void INT0_FallingEdge_ISR(void) __interrupt (INT_NO_INT0);
void Goodix_setHandler(void (*__xdata handler)(int8_t, GTPoint_t*));
void Goodix_loop(void);
int16_t Goodix_readInput(uint8_t *data);
GTInfo_t* Goodix_readInfo(void);
void command(uint16_t reg_addr, uint8_t hex_command);



#endif /* CORE_INC_GT911_H_ */
