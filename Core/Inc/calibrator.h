#ifndef CORE_INC_CALIBRATOR_H_
#define CORE_INC_CALIBRATOR_H_

#include <stdbool.h>
#include "gt911_structs.h"
#include "ch554.h"


#define LED_PIN 4
#define BUTTON_PIN 3

SBIT(LED, 0xB0, LED_PIN);
SBIT(BUTTON, 0xB0, BUTTON_PIN);


#define CALIBRATE_DDR DDRD
#define CALIBRATE_IN PIND
//#define CALIBRATE_PORT PORTD
#define CALIBRATE_PIN_BTN BUTTON
//#define CALIBRATE_PIN_LED PORTD5
#define CALIBRATE_PIN_LED LED

void Calibrator_init(void);
bool Calibrator_adjust(int8_t contacts, GTPoint_t *points);
void Calibrator_tic(void);

#endif /* CORE_INC_CALIBRATOR_H_ */
