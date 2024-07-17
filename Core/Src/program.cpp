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
uint8_t ImuDataPacket[20]={0};
float pitch;
float roll;
float yaw;
/*************************Nesneler**************************/
Uart uart3(USART3);
Timer timer7(TIM3);
Gorevyonetici gorev(TIM3);
MyImu imu(&hi2c1);
GPS gps(&huart2);
Paket gpsPaket(0x12, 0x34, 0x01, 0x08); //veri boyutu 8
Paket imuPaket(0x12, 0x34, 0x02, 0x10);//veri boyutu 16
/*****************Fonksiyon Bildirimleri********************/
void Gorevler1();
void Gorevler2();
void Gorevler3();

void setup()
{
	uart3.Yapilandir(115200, GPIOD, GPIO_PIN_8,GPIOB ,GPIO_PIN_11);
	gps.Yapilandir();
	imu.DBC_MPU6500_YAPILANDIR();
	imu.DBC_GYRO_OFSET();
	timer7.Yapilandir(84000,5);
	timer7.AktifEt();

	gorev.GorevAl(Gorevler2,25);
	gorev.GorevAl(Gorevler3,500);
	gorev.GorevAl(Gorevler1,1000);
}

void loop()
{
	gorev.GorevCalistir();
}

void Gorevler1()
{
	//gps.LatitudeAl();
	//gps.LongitudeAl();
	GPIOD->ODR ^= GPIO_PIN_12;
	gpsPaket.PaketOlustur(*gps.LatitudeAl(),*gps.LongitudeAl());
	gpsPaket.gpsPaketCagir(GpsDataPacket);
	uart3.transmit(GpsDataPacket, sizeof(GpsDataPacket));
}
void Gorevler2()
{
	GPIOD->ODR ^= GPIO_PIN_13;
	imu.DBC_ACI_BULMA();
	pitch=*imu.PitchAl();
	roll=*imu.RollAl();
	yaw=*imu.YawAl();
	imuPaket.PaketOlustur(*imu.PitchAl(), *imu.RollAl(), *imu.YawAl(), *imu.SicaklikAl());
	imuPaket.imuPaketCagir(ImuDataPacket);

}
void Gorevler3()
{
	uart3.transmit(ImuDataPacket, sizeof(ImuDataPacket));
}
extern "C" void TIM3_IRQHandler()
{
	gorev.BayrakKaldir();
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	gps.UartRxCpltCallback();
}

