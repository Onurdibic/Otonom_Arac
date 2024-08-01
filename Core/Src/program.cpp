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
#include <cmath>
#include "tim.h"
#include "Motor.h"
#include "Barometre.h"
/************************Degiskenler************************/
float aci;
int8_t tur;
uint8_t GpsDataPacket[21]={0};
uint8_t ImuDataPacket[21]={0};
float pitch;
float roll;
float yaw;
float heading;
/*************************Nesneler**************************/
//Uart uart3(USART3);

Timer timer3(TIM3);
//Timer timer2(TIM2);
Gorevyonetici gorev(TIM3);
MyImu imu(&hi2c1);
GPS gps(&huart2);
MyMag mag(&hi2c1);
Barometre barometre(&hi2c1, 0xEE);
Motor motor1(&htim2, &htim1, GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_5);
Paket GpsPaket(0x12, 0x34, 0x01, 0x09); //veri boyutu 9
Paket ImuPaket(0x12, 0x34, 0x02, 0x11);//veri boyutu 17
Paket ArayuzPaket(&huart3);
Araba araba;
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
	motor1.Yapilandir();
	gps.Yapilandir();
	mag.Yapilandir();
	imu.DBC_MPU6500_YAPILANDIR();
	barometre.Yapilandir();
	timer3.Yapilandir(84000,5);
	timer3.AktifEt();

	imu.DBC_GYRO_OFSET();
	GPIOD->ODR ^= GPIO_PIN_12;
	mag.KalibreEt();
	gorev.GorevAl(Gorevler2,25);
	gorev.GorevAl(Gorevler3,500);
	gorev.GorevAl(Gorevler1,1000);
}

void loop()
{
	motor1.PWM(500);
	motor1.AciBul();
	tur=*motor1.TurAl();
	aci=*motor1.AciAl();

	gorev.GorevCalistir();
	 if(ArayuzPaket.PaketCozBayrak)
	 {
		 ArayuzPaket.PaketCoz();
		 ArayuzPaket.PaketCozBayrak = false;
		 if(ArayuzPaket.MesafeBayrak)
		 {
			 araba.mesafeBul(*gps.LatitudeAl(),*gps.LongitudeAl(), *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLatAl());
			 araba.yonelimBul(*gps.LatitudeAl(),*gps.LongitudeAl(), *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLatAl());
		 }
	 }
}

void Gorevler1()
{

	GPIOD->ODR ^= GPIO_PIN_12;
	GpsPaket.GpsPaketOlustur(*gps.LatitudeAl(),*gps.LongitudeAl(),*barometre.IrtifaOku(0),*barometre.SicaklikOku());
	GpsPaket.gpsPaketCagir(GpsDataPacket);
	if(ArayuzPaket.YoklamaFlag)
	{
		HAL_UART_Transmit(&huart3, GpsDataPacket, sizeof(GpsDataPacket), 1000);
	}

	//HAL_UART_Transmit(&huart3, ImuDataPacket, sizeof(ImuDataPacket), HAL_MAX_DELAY);
}
void Gorevler2()
{
	GPIOD->ODR ^= GPIO_PIN_13;
	imu.DBC_ACI_BULMA();

	pitch=*imu.PitchAl();
	roll=*imu.RollAl();
	yaw=*imu.YawAl();
	heading = *mag.HeadingOlustur();
	ImuPaket.ImuPaketOlustur(*imu.PitchAl(), *imu.RollAl(), *mag.HeadingOlustur(), *imu.SicaklikAl());
	ImuPaket.imuPaketCagir(ImuDataPacket);
}
void Gorevler3()
{
	if(ArayuzPaket.YoklamaFlag)
	{
		HAL_UART_Transmit(&huart3, ImuDataPacket, sizeof(ImuDataPacket), 1000);
	}

}
extern "C" void TIM3_IRQHandler()
{
	gorev.BayrakKaldir();
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		gps.UartRxCpltCallback();
	}
	if (huart->Instance == USART3)
	{
		ArayuzPaket.BayrakKaldir();
		//HAL_UART_Receive_IT(&huart3, ArayuzData, sizeof(ArayuzData));

	}
}
/*
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  if (GPIO_Pin == GPIO_PIN_7)
  {
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7) == HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8))
    {
      encoderDegeriA_s32++;
    }
    else
    {
      encoderDegeriA_s32--;
    }
  }

  if (GPIO_Pin == GPIO_PIN_8)
    {
      if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7) == HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8))
      {
        encoderDegeriB_s32++;
      }
      else
      {
        encoderDegeriB_s32--;
      }
    }
  encoderDegeri_s32 = encoderDegeriA_s32-encoderDegeriB_s32;
  aci = ((float)encoderDegeri_s32 * 0.02197);
  //aci = fmod(((float)encoderDegeri_s32 * 0.0219), 360.0);
  count = ((float)encoderDegeri_s32 * 0.0219)/ 360.0;
}*/
