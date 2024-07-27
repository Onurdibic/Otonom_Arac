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
uint8_t GpsDataPacket[14]={0};
uint8_t ImuDataPacket[21]={0};
float temp1,altitude1,press1;
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
MyMag mag(&hi2c3);
Barometre barometre(&hi2c2, 0xEE);
Motor motor1(&htim2, &htim1, GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_5);
Paket GpsPaket(0x12, 0x34, 0x01, 0x09); //veri boyutu 9
Paket ImuPaket(0x12, 0x34, 0x02, 0x11);//veri boyutu 17
Paket ArayuzPaket(&huart3);
/*****************Fonksiyon Bildirimleri********************/
void Gorevler1();
void Gorevler2();
void Gorevler3();

void setup()
{
	motor1.Yapilandir();
	//uart3.Yapilandir(115200, GPIOD, GPIO_PIN_8,GPIOB ,GPIO_PIN_11);
	//uart3.receiveIT(RotaData, 8);
	//HAL_UART_Receive_IT(&huart3, ArayuzData, sizeof(ArayuzData));
	ArayuzPaket.PaketKesmeInit();
	gps.Yapilandir();
	mag.Yapilandir();
	//mag.KalibreEt();
	imu.DBC_MPU6500_YAPILANDIR();
	imu.DBC_GYRO_OFSET();
	timer3.Yapilandir(84000,5);
	timer3.AktifEt();
	barometre.Yapilandir();
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
	 }
}

void Gorevler1()
{

	GPIOD->ODR ^= GPIO_PIN_12;
	GpsPaket.PaketOlustur(*gps.LatitudeAl(),*gps.LongitudeAl());
	GpsPaket.gpsPaketCagir(GpsDataPacket);
	HAL_UART_Transmit(&huart3, GpsDataPacket, sizeof(GpsDataPacket), HAL_MAX_DELAY);
	//HAL_UART_Transmit(&huart3, ImuDataPacket, sizeof(ImuDataPacket), HAL_MAX_DELAY);
}
void Gorevler2()
{
	GPIOD->ODR ^= GPIO_PIN_13;
	imu.DBC_ACI_BULMA();
	heading = *mag.HeadingOlustur();
	pitch=*imu.PitchAl();
	roll=*imu.RollAl();
	yaw=*imu.YawAl();
	ImuPaket.PaketOlustur(*imu.PitchAl(), *imu.RollAl(), *mag.HeadingOlustur(), *imu.SicaklikAl());
	ImuPaket.imuPaketCagir(ImuDataPacket);
}
void Gorevler3()
{
	HAL_UART_Transmit(&huart3, ImuDataPacket, sizeof(ImuDataPacket), HAL_MAX_DELAY);
	temp1 = barometre.SicaklikOku();
	press1 = barometre.BasincOku(0);
	altitude1 = barometre.IrtifaOku(0);
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
