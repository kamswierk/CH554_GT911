#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ch554.h"
#include "i2c.h"
#include "ch554_usb.h"

#include "gt911.h"
#include "gt911_reg.h"
#include "gt911_if.h"
#include "gt911_structs.h"


#ifdef DEBUG
#include "debug.h"
#endif // DEBUG



// Interrupt INT0 flag
volatile uint8_t goodixIRQ = 0;

// Variables
GTConfig_t config;
GTInfo_t info;
uint8_t points[GOODIX_MAX_CONTACTS*GOODIX_CONTACT_SIZE]; //points buffer

// Callback function pointer
void (*__xdata touchHandler)(int8_t, GTPoint_t*) __reentrant; //reetnrtant?

// Callback register
void Goodix_setHandler(void (* __xdata handler)(int8_t, GTPoint_t*) __reentrant) {
  touchHandler = handler;
}

// Define your ISR function for INT0 falling edge
void INT0_FallingEdge_ISR(void) __interrupt (INT_NO_INT0) // @suppress("Unused function declaration")
{
    IE0 = 0;
    goodixIRQ = 1;
}


void onIRQ(void)
{
//	uint8_t k;
	//uint8_t buf[1] + GOODIX_CONTACT_SIZE * GOODIX_MAX_CONTACTS];
	int8_t contacts;

	contacts = Goodix_readInput(points);
	if (contacts < 0)
	return;

//	for(k=0;k<8;k++)
//	{
//		printf("0x%X ", points[k]);
//		printf("%d ", points[k]);
//
//	}
//
//	if(contacts == 1)
//	{
//		for(k=0;k<8;k++)
//		{
//			printf("%d ", points[k]);
//
//		}
//
//		printf("x: %d ",((GTPoint_t *)points)[0].x);
//		printf("y: %d",((GTPoint_t *)points)[0].y);
//		printf("\n\r");
//	}
//	else if(contacts == 2)
//	{
//		for(k=0;k<16;k++)
//		{
//			printf("%d ", points[k]);
//
//		}
//
//		printf("x1: %d ",((GTPoint_t *)points)[0].x);
//		printf("y1: %d ",((GTPoint_t *)points)[0].y);
//
//		printf("x2: %d ",((GTPoint_t *)points)[1].x);
//		printf("y2: %d ",((GTPoint_t *)points)[1].y);
//		printf("\n\r");
//	}
//	else if(contacts == 3)
//	{
//		for(k=0;k<24;k++)
//		{
//			printf("%d ", points[k]);
//
//		}
//
//		printf("x1: %d ",((GTPoint_t *)points)[0].x);
//		printf("y1: %d ",((GTPoint_t *)points)[0].y);
//
//		printf("x2: %d ",((GTPoint_t *)points)[1].x);
//		printf("y2: %d ",((GTPoint_t *)points)[1].y);
//
//		printf("x3: %d ",((GTPoint_t *)points)[2].x);
//		printf("y3: %d ",((GTPoint_t *)points)[2].y);
//		printf("\n\r");
//	}

	touchHandler(contacts, (GTPoint_t *)points);

//	write(GOODIX_READ_COORD_ADDR, 0);
	uint8_t WriteValue = 0;
	GT911_WriteToI2C(GOODIX_I2C_ADDR_28, GOODIX_READ_COORD_ADDR, &WriteValue, 1);
}


void Goodix_loop(void) {
	if (goodixIRQ) {
		onIRQ();
//		Enp2BulkIn();
		goodixIRQ = 0;
	}
}

#define EAGAIN 100 // Try again error


GTInfo_t* Goodix_readInfo(void)
{
	GT911_ReadFromI2C(GOODIX_I2C_ADDR_28, GT_REG_DATA, (uint8_t *) &info, sizeof(config));

	return &info;
}


int16_t Goodix_readInput(uint8_t *data) {
  int touch_num;
  int error;

  uint8_t regState[1];

  error = GT911_ReadFromI2C(GOODIX_I2C_ADDR_28, GOODIX_READ_COORD_ADDR, regState, 1);

#ifdef DEBUG
//  printf("regState: 0x%X\n\r", regState);
#endif

  if (error) {
    //dev_err(&ts->client->dev, "I2C transfer error: %d\n", error);
    return -error;
  }

  if (!(regState[0] & 0x80))
    return -EAGAIN;

  touch_num = regState[0] & 0x0f;
  //if (touch_num > ts->max_touch_num)
  //  return -EPROTO;

#ifdef DEBUG
//printf("touch num: %d\n\r", touch_num);
#endif

  if (touch_num > 0) {
    /*    data += 1 + GOODIX_CONTACT_SIZE;
        error = read(GOODIX_READ_COORD_ADDR + 1 + GOODIX_CONTACT_SIZE, data,
              GOODIX_CONTACT_SIZE * (touch_num - 1));
    */
    error = GT911_ReadFromI2C(GOODIX_I2C_ADDR_28,GOODIX_READ_COORD_ADDR + 1, data, GOODIX_CONTACT_SIZE * (touch_num));

    if (error)
      return -error;
  }

  return touch_num;
}
