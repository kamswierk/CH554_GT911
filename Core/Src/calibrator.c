#include <stdint.h>
#include <stdio.h>
#include <float.h>
#include "calibrator.h"
#include "DataFlash.h"
#include "delay.h"

#include "gt911.h"
#include "gt911_structs.h"

#ifdef DEBUG
#include "debug.h"
#endif // DEBUG


#define LOGICAL_MAX_X 2047.0F
#define LOGICAL_MAX_Y 2047.0F

#define STAGE_IDLE		0
#define STAGE_WAIT_P0	1
#define STAGE_CLICK_P0	2
#define STAGE_WAIT_P1	3
#define STAGE_CLICK_P1	4
#define STAGE_WAIT_P2	5
#define STAGE_CLICK_P2	6

#define BTN_HOLD_TIME 100 // 1sec

volatile uint8_t calibrationStage = 0;
volatile GTPoint_t calibratePoints[3];


volatile float xMul = 2.0F;
volatile float yMul = 3.41F;


uint16_t adjust(uint16_t raw, float mul)
{
	if(mul == 1.0F)
	{
		return raw;
	}

	return raw * mul;
}


uint16_t diff(uint16_t one, uint64_t other)
{
	if(one > other)
	{
		return one - other;
	}

	return other - one;
}


void Calibrator_init(void) {
	// IO
	// PORT 3 PIN 3 (LED) OUTPUT MODE: PUSH-PULL OUTPUT
	P3_MOD_OC = P3_MOD_OC & ~(1 << LED_PIN);
	P3_DIR_PU = P3_DIR_PU |	(1 << LED_PIN);
	CALIBRATE_PIN_LED = 0; //led on

	// PORT 3 PIN 3 (BUTTON) INPUT WITH PULL-UP RESISTOR
	P3_MOD_OC = P3_MOD_OC | (1 << BUTTON_PIN);
	P3_DIR_PU = P3_DIR_PU | (1 << BUTTON_PIN);

//	ReadDataFlash(0xC000, 4, &xMul);
//	ReadDataFlash(0xC008, 4, &yMul);
//	xMul = 2.0F;
//	yMul = 3.41F;

	if(xMul < 1 || yMul < 1 || xMul > 100 || yMul > 100) { // not a valid value
		printf("Nie powinno robic tego gowna\n\r");
		volatile GTInfo_t* info = Goodix_readInfo();
		xMul = LOGICAL_MAX_X / info->xResolution;
		yMul = LOGICAL_MAX_Y / info->yResolution;
	}
}


void calibrateEnd(void)
{
	volatile uint16_t physicalWidth = diff(calibratePoints[0].x, calibratePoints[1].x);
	xMul = LOGICAL_MAX_X / physicalWidth;
	WriteDataFlash(0xC000, &xMul, 4);  // update float

	volatile uint16_t physicalHeight = diff(calibratePoints[1].y, calibratePoints[2].y);
	yMul = LOGICAL_MAX_Y / physicalHeight;
	WriteDataFlash(0xC008, &yMul, 4);  // update float

	CALIBRATE_PIN_LED = 0;

	calibrationStage = STAGE_IDLE;
}


bool Calibrator_adjust(int8_t contacts, GTPoint_t *points)
{
//	printf("Calibration stage: %d\n\r", calibrationStage);
	if(calibrationStage == STAGE_IDLE)
	{
		for (int8_t i = 0; i < contacts; i++)
		{
//			printf("X Before adjust: %d xMul: %u\n\r", points[i].x, __fs2uchar(xMul));
//			printf("Y Before adjust: %d yMul: %u\n\r", points[i].y, __fs2uchar(yMul));
			points[i].x = adjust(points[i].x, xMul);
			points[i].y = adjust(points[i].y, yMul);
//			printf("X After adjust: %d xMul: %u\n\r", points[i].x, __fs2uchar(xMul));
//			printf("Y After adjust: %d yMul: %u\n\r", points[i].y, __fs2uchar(yMul));
		}
		return true;
	}
	// led notification (turn off when pressed)
	if(contacts == 0)
	{
		CALIBRATE_PIN_LED = 1;
	}
	else
	{
		CALIBRATE_PIN_LED = 0;
	}

	switch(calibrationStage)
	{
		case STAGE_WAIT_P0: //		#1
			if(contacts > 0)
			{
				calibrationStage = STAGE_CLICK_P0;
				calibratePoints[0] = points[0];
			}
			break;

		case STAGE_CLICK_P0: //		#2
			if(contacts == 0)
			{
				calibrationStage = STAGE_WAIT_P1;
			}
			else
			{
				calibratePoints[0] = points[0];
			}
			break;

		case STAGE_WAIT_P1: //		#3
			if(contacts > 0)
			{
				calibrationStage = STAGE_CLICK_P1;
				calibratePoints[1] = points[0];
			}
			break;

		case STAGE_CLICK_P1: //		#4
			if(contacts == 0)
			{
				calibrationStage = STAGE_WAIT_P2;
			}
			else
			{
				calibratePoints[1] = points[0];
			}
			break;

		case STAGE_WAIT_P2: //		#5
			if(contacts > 0)
			{
				calibrationStage = STAGE_CLICK_P2;
				calibratePoints[2] = points[0];
			}
			break;

		case STAGE_CLICK_P2: //		#6
			if(contacts == 0)
			{
				calibrateEnd();
			}
			else
			{
				calibratePoints[2] = points[0];
			}
			break;

	}
	return false;
}


void calibrateStart(void)
{
	CALIBRATE_PIN_LED = 1; // led off
	calibrationStage = STAGE_WAIT_P0;
}


void Calibrator_tic(void)
{
	printf("calibrationStage: %d\n\r", calibrationStage);
	if(calibrationStage == STAGE_IDLE)
	{
		calibrateStart();
	}
}


