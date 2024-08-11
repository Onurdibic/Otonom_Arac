/*
 * Motor.h
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */
#ifndef MYOTONOMDRIVERS_INC_MOTOR_H_
#define MYOTONOMDRIVERS_INC_MOTOR_H_

#include "stm32f4xx_hal.h"
#include <math.h>
#define DUNYA_YARICAPI 6371000.0
class Motor
{
public:
	Motor(TIM_HandleTypeDef* htim_pwm, TIM_HandleTypeDef* htim_enc, GPIO_TypeDef* in1_port, uint16_t in1_pin, GPIO_TypeDef* in2_port, uint16_t in2_pin);
	void Yapilandir();
	void PWM(uint16_t pwmDeger_u16,uint32_t channel_u32);
	void AciBul();
	void IleriGit();
	void GeriGit();
	void Dur();
	float* AciAl();
	int8_t* TurAl();

private:
	  TIM_HandleTypeDef* htim_pwm_;
	  TIM_HandleTypeDef* htim_enc_;
	  GPIO_TypeDef* in1_port_;
	  GPIO_TypeDef* in2_port_;
	  uint16_t in1_pin_;
	  uint16_t in2_pin_;
	  float motorAci_f=0;
	  int64_t toplamEncoderDegeri_s64 = 0;
	  int16_t oncekiEncoderDegeri_s16 = 0;
	  int16_t simdikiEncoderDegeri_s16 = 0;
	  int8_t motorTur_u8=0;

};

class Araba
{
public:

	//Araba();
	Araba(Motor& lmotor,Motor& rmotor);
	void Dur();
	void duzGit();
	void sagGit();
	void solGit();

	float mesafeBul(float guncelLat_f, float guncelLon_f, float gidilecekLat_f, float gidilecekLon_f);
	float yonelimBul(float guncelLat_f, float guncelLon_f, float gidilecekLat_f, float gidilecekLon_f);
private:
	Motor& lm;
	Motor& rm;

};



#endif /* MYOTONOMDRIVERS_INC_MOTOR_H_ */
