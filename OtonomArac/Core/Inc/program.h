/*
 * program.h
 *
 *  Created on: Jul 14, 2024
 *      Author: onurd
 */

#ifndef INC_PROGRAM_H_
#define INC_PROGRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

void setup();
void loop();

#ifdef __cplusplus
}
#endif

#endif /* INC_PROGRAM_H_ */
