/*
 * VoltajSensor.cpp
 *
 *  Created on: Aug 16, 2024
 *      Author: onurd
 */
#include "VoltajSensor.h"


VoltajSensor::VoltajSensor(ADC_HandleTypeDef *hadc): hadc(hadc){}

void VoltajSensor::Yapilandir()
{
	//HAL_ADC_Start(hadc);
}
float VoltajSensor::Olc()
{
	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, 100);
	adcData_u32=HAL_ADC_GetValue(hadc);
	voltaj_f=(float)adcData_u32/4095.0*16.6;
	return voltaj_f;
}
float VoltajSensor::BataryaYuzdeBul()
{
	voltaj_f = Olc();
	bataryaYuzde_f = (voltaj_f - 12.4) / 4.4 * 100.0;

	bataryaYuzde_f=adcIsaret.altSinirla(bataryaYuzde_f, 0);
	bataryaYuzde_f=adcIsaret.ustSinirla(bataryaYuzde_f, 100);

	return bataryaYuzde_f;
}
