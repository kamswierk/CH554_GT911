/*
 * delay.h
 *
 *  Created on: 18 mar 2024
 *      Author: DELLPC
 */

#ifndef CORE_INC_DELAY_H_
#define CORE_INC_DELAY_H_

#include <stdint.h>

void CfgFsys(void);
void mDelayuS (uint16_t n); // Delay in units of uS
void mDelaymS (uint16_t n); // Delay in mS

#endif /* CORE_INC_DELAY_H_ */
