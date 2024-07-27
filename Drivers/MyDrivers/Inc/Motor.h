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

class Motor
{
public:
	Motor(TIM_HandleTypeDef* htim_pwm, TIM_HandleTypeDef* htim_enc, GPIO_TypeDef* in1_port, uint16_t in1_pin, GPIO_TypeDef* in2_port, uint16_t in2_pin);
	void Yapilandir();
	void PWM(uint16_t pwmDeger_u16);
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

class Araba{
public:
	Araba(Motor& olmotor,Motor& ormotor,Motor& almotor,Motor& armotor);
	void duzGit();
	void sagGit();
	void solGit();
private:
	Motor& olm;
	Motor& orm;
	Motor& alm;
	Motor& arm;

};



#endif /* MYOTONOMDRIVERS_INC_MOTOR_H_ */
