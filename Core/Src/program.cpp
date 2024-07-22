/*
 * program.cpp
 *
 *  Created on: Jul 14, 2024
 *      Author: onurd
 */
#include "program.h"

#include "Paket.h"
#include "mytimer.h"
#include "gorevyonetici.h"
#include "Gps.h"
#include "Imu.h"
#include "usart.h"
#include "Mag.h"

/************************Degiskenler************************/
uint8_t GpsDataPacket[14]={0};
uint8_t ImuDataPacket[21]={0};
uint8_t ArayuzData[120]={0};
float pitch;
float roll;
float yaw;
int16_t x, y, z;
float heading;
/*************************Nesneler**************************/
//Uart uart3(USART3);
Timer timer7(TIM3);
Gorevyonetici gorev(TIM3);
MyImu imu(&hi2c1);
GPS gps(&huart2);
HMC5883L mag(&hi2c3);

Paket GpsPaket(0x12, 0x34, 0x01, 0x09); //veri boyutu 9
Paket ImuPaket(0x12, 0x34, 0x02, 0x11);//veri boyutu 17
Paket ArayuzPaket(&huart3);
/*****************Fonksiyon Bildirimleri********************/
void Gorevler1();
void Gorevler2();
void Gorevler3();

void setup()
{
	//uart3.Yapilandir(115200, GPIOD, GPIO_PIN_8,GPIOB ,GPIO_PIN_11);
	//uart3.receiveIT(RotaData, 8);
	//HAL_UART_Receive_IT(&huart3, ArayuzData, sizeof(ArayuzData));
	ArayuzPaket.PaketKesmeInit();
	gps.Yapilandir();
	mag.Yapilandir();
	mag.KalibreEt();
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
	if(ArayuzPaket.PaketCozBayrak)
	{
		ArayuzPaket.PaketCoz();
		ArayuzPaket.PaketCozBayrak = false;
	}
}

void Gorevler1()
{
	//gps.LatitudeAl();
	//gps.LongitudeAl();
	GPIOD->ODR ^= GPIO_PIN_12;
	GpsPaket.PaketOlustur(*gps.LatitudeAl(),*gps.LongitudeAl());
	GpsPaket.gpsPaketCagir(GpsDataPacket);
	//uart3.transmit(GpsDataPacket, sizeof(GpsDataPacket));
	HAL_UART_Transmit(&huart3, GpsDataPacket, sizeof(GpsDataPacket), HAL_MAX_DELAY);
}
void Gorevler2()
{
	GPIOD->ODR ^= GPIO_PIN_13;
	imu.DBC_ACI_BULMA();
	heading = mag.HeadingOlustur();
	pitch=*imu.PitchAl();
	roll=*imu.RollAl();
	yaw=*imu.YawAl();
	ImuPaket.PaketOlustur(*imu.PitchAl(), *imu.RollAl(), *imu.YawAl(), *imu.SicaklikAl());
	ImuPaket.imuPaketCagir(ImuDataPacket);
}
void Gorevler3()
{
	HAL_UART_Transmit(&huart3, ImuDataPacket, sizeof(ImuDataPacket), HAL_MAX_DELAY);

}
extern "C" void TIM3_IRQHandler()
{
	gorev.BayrakKaldir();
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	    {
	       gps.UartRxCpltCallback(); // Your existing callback for USART2
	    }
	    if (huart->Instance == USART3)
	    {
	    	ArayuzPaket.BayrakKaldir();
	    	//HAL_UART_Receive_IT(&huart3, ArayuzData, sizeof(ArayuzData));

	    }
}

