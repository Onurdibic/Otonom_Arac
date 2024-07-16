/*
 * program.cpp
 *
 *  Created on: Jul 14, 2024
 *      Author: onurd
 */
#include "program.h"

#include "Paket.h"
#include "mytimer.h"
#include "myUart.h"
#include "gorevyonetici.h"
#include "Gps.h"
#include "Imu.h"


/************************Degiskenler************************/
uint8_t GpsDataPacket[12]={0};

/*************************Nesneler**************************/
Uart uart3(USART3);
Timer timer7(TIM3);
Gorevyonetici gorev(TIM3);
MyImu imu(&hi2c1);
GPS gps(&huart2);
Paket gpsPaket(0x12, 0x34, 0x01, 0x08);

/*****************Fonksiyon Bildirimleri********************/
void Gorevler();
void Gorevler1();


void setup()
{
	uart3.Yapilandir(115200, GPIOD, GPIO_PIN_8,GPIOB ,GPIO_PIN_11);
	gps.Yapilandir();
	timer7.Yapilandir(84000,5);
	timer7.AktifEt();
	gorev.GorevAl(Gorevler,1000);
	gorev.GorevAl(Gorevler1, 500);
}

void loop()
{
	gorev.GorevCalistir();
}

void Gorevler()
{
	//gps.LatitudeAl();
	//gps.LongitudeAl();
	GPIOD->ODR ^= GPIO_PIN_12;
	gpsPaket.PaketOlustur(*gps.LatitudeAl(),*gps.LongitudeAl());
	gpsPaket.PaketCagir(GpsDataPacket);
	uart3.transmit(GpsDataPacket, sizeof(GpsDataPacket));
}
void Gorevler1()
{
	GPIOD->ODR ^= GPIO_PIN_13;
}

extern "C" void TIM3_IRQHandler()
{
	{
		gorev.BayrakKaldir();
	}
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	gps.UartRxCpltCallback();
}

