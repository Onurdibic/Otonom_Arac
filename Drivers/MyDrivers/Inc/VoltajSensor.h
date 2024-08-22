/*
 * VoltajSensor.h
 *
 *  Created on: Aug 16, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_VOLTAJSENSOR_H_
#define MYDRIVERS_INC_VOLTAJSENSOR_H_

#include "stm32f4xx_hal.h"

#include "IsaretIsleme.h"
class VoltajSensor
{
public:
	VoltajSensor(ADC_HandleTypeDef *hadc);
	void Yapilandir();
	float Olc();
	float BataryaYuzdeBul();
private:
	ADC_HandleTypeDef *hadc;

	Isaret adcIsaret;
	uint32_t adcData_u32=0;
	float voltaj_f=0;
	float bataryaYuzde_f=0;
};



#endif /* MYDRIVERS_INC_VOLTAJSENSOR_H_ */
