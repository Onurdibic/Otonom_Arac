/*
 * Motor.cpp
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */
#include "Motor.h"
Motor::Motor(TIM_HandleTypeDef* htim_pwm, TIM_HandleTypeDef* htim_enc, GPIO_TypeDef* in1_port, uint16_t in1_pin, GPIO_TypeDef* in2_port, uint16_t in2_pin)
{
	 htim_pwm_ = htim_pwm;
	 htim_enc_ = htim_enc;
	 in1_port_ = in1_port;
	 in1_pin_ = in1_pin;
	 in2_port_ = in2_port;
	 in2_pin_ = in2_pin;
}
void Motor::Yapilandir()
{
    HAL_TIM_PWM_Start(htim_pwm_, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(htim_enc_, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(htim_enc_, 0);
}
void Motor::PWM(uint16_t pwmDeger_u16)
{
	__HAL_TIM_SET_COMPARE(htim_pwm_, TIM_CHANNEL_1, pwmDeger_u16); // (0-1000)
}
void Motor::AciBul()
{
	simdikiEncoderDegeri_s16 = __HAL_TIM_GET_COUNTER(htim_enc_);
	int16_t delta = simdikiEncoderDegeri_s16 - oncekiEncoderDegeri_s16;

	IleriGit();
	// Encoder taşması kontrolü
	if (delta < -32767)
	{
		delta += 65536; // örnek onceki 32760 simdiki -32760 delta -65520 adım 16
	}
	else if (delta > 32767)
	{
		delta -= 65536; // örnek onceki -32740 simdiki 32750 delta 65490 adım 46
	}

	toplamEncoderDegeri_s64 += delta;
	oncekiEncoderDegeri_s16 = simdikiEncoderDegeri_s16;

	motorAci_f = fmod(((float)toplamEncoderDegeri_s64 * 0.02347), 360.0);
	motorTur_u8 = ((float)toplamEncoderDegeri_s64 * 0.02347)/ 360.0;
}
void Motor::IleriGit()
{
	 HAL_GPIO_WritePin(in1_port_, in1_pin_, GPIO_PIN_SET); // IN1
	 HAL_GPIO_WritePin(in2_port_, in2_pin_, GPIO_PIN_RESET); // IN2
}
void Motor::GeriGit()
{
	 HAL_GPIO_WritePin(in1_port_, in1_pin_, GPIO_PIN_RESET); // IN1
	 HAL_GPIO_WritePin(in2_port_, in2_pin_,GPIO_PIN_SET); // IN2
}
void Motor::Dur()
{
	HAL_GPIO_WritePin(in1_port_, in1_pin_, GPIO_PIN_RESET); // IN1
	HAL_GPIO_WritePin(in2_port_, in2_pin_, GPIO_PIN_RESET); // IN2
}
float* Motor::AciAl()
{
    return &motorAci_f;
}

int8_t* Motor::TurAl()
{
    return &motorTur_u8;
}
Araba::Araba(Motor& olmotor,Motor& ormotor,Motor& almotor,Motor& armotor)
		:olm(olmotor),orm(ormotor),alm(almotor),arm(armotor){}

void Araba::duzGit()
{
	 olm.IleriGit();
	 orm.IleriGit();
	 alm.IleriGit();
	 arm.IleriGit();
}
void Araba::sagGit()
{
	  olm.IleriGit();
	  orm.Dur();
	  alm.IleriGit();
	  arm.Dur();
}
void Araba::solGit()
{
    olm.Dur();
    orm.IleriGit();
    alm.Dur();
    arm.IleriGit();
}
