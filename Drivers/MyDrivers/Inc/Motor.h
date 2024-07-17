/*
 * Motor.h
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */
#include "stm32f4xx_hal.h"
#include "stdint.h"
#ifndef MYOTONOMDRIVERS_INC_MOTOR_H_
#define MYOTONOMDRIVERS_INC_MOTOR_H_

class Motor{
public:
	Motor();
	void init(int pin);
	void git(int aci);
	void dur();
private:


};

class Araba: public Motor{
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
