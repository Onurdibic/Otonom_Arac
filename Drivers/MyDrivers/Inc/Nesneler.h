/*
 * Nesneler.h
 *
 *  Created on: Aug 9, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_NESNELER_H_
#define MYDRIVERS_INC_NESNELER_H_

#include "Motor.h"
#include "Barometre.h"
#include "Paket.h"
#include "mytimer.h"
#include "gorevyonetici.h"
#include "Gps.h"
#include "Imu.h"

/*************************Nesneler**************************/
//Uart uart3(USART3);

Timer timer3(TIM3);
//Timer timer2(TIM2);
Gorevyonetici gorev(TIM3);
MyImu imu(&hi2c1);
GPS gps(&huart2);
MyMag mag(&hi2c1);
Barometre barometre(&hi2c1, 0xEE);
Motor motorSag(&htim2, &htim1, GPIOD, GPIO_PIN_1, GPIOD, GPIO_PIN_2);
Motor motorSol(&htim2, &htim1, GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_5);

Paket GpsPaket(0x12, 0x34, 0x01, 0x11); //veri boyutu 17
Paket ImuPaket(0x12, 0x34, 0x02, 0x11);//veri boyutu 17
Paket ArayuzPaket(&huart3);
Araba araba(motorSol,motorSag);



#endif /* MYDRIVERS_INC_NESNELER_H_ */
