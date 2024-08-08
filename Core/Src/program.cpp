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
#include "Nesneler.h"
/************************Degiskenler************************/
float Kp = 50;
float hata = 0;
float kontrolSinyali;
float Ki = 0.1;
float integral = 0;
float prevHata = 0;
float aci;
int8_t tur;
uint8_t GpsDataPacket[21]={0};
uint8_t ImuDataPacket[21]={0};
float pitch;
float roll;
float yaw;
float heading;
float irtifa;
float gidilecekMesafe_f=0;
float donulecekAci_f =0;
float baslangicHatasi=0;
float baslangicHeading=0;
bool hedefbayrak=true;
bool solaDonbayrak=false;
bool sagaDonbayrak=false;
enum ArabaDurumlar
{
	MesafeBul,
	YonelimBul,
	Yonel,
	Ilerle,
	Var
};
/*****************Fonksiyon Bildirimleri********************/
void GpsGorev();
void ImuGorev();
void YonelmeGorev();
void Gorevler4();
void ArayuzTask();

void setup()
{
	//uart3.Yapilandir(115200, GPIOD, GPIO_PIN_8,GPIOB ,GPIO_PIN_11);
	//uart3.receiveIT(RotaData, 8);

	ArayuzPaket.PaketKesmeInit();
	motorSag.Yapilandir();
	motorSol.Yapilandir();
	gps.Yapilandir();
	mag.Yapilandir();
	imu.DBC_MPU6500_YAPILANDIR();
	barometre.Yapilandir();
	timer3.Yapilandir(84000,5);
	timer3.AktifEt();

	HAL_Delay(1000);
	GPIOD->ODR ^= GPIO_PIN_12;
	mag.KalibreEt();
	GPIOD->ODR ^= GPIO_PIN_12;
	HAL_Delay(2000);
	imu.DBC_GYRO_OFSET();
	gorev.GorevAl(ImuGorev,30);
	gorev.GorevAl(ArayuzTask,100);
	gorev.GorevAl(YonelmeGorev,500);
	gorev.GorevAl(GpsGorev,1000);
	motorSag.PWM(1000,TIM_CHANNEL_2);
	motorSol.PWM(1000,TIM_CHANNEL_4);
	araba.Dur();

}

void loop()
{
	//motor1.AciBul();
	//tur=*motor1.TurAl();
	//aci=*motor1.AciAl();

	gorev.GorevCalistir();

	if(ArayuzPaket.GidilecekNoktaBayrak)
	{
		if(solaDonbayrak==true)
		{
			araba.solGit();
			solaDonbayrak=false;
		}
		if(sagaDonbayrak==true)
		{
			araba.sagGit();
			sagaDonbayrak=false;
		}
		if(heading >= donulecekAci_f - 4 && heading <= donulecekAci_f + 4)
		{
			araba.duzGit();
		}
	}

}

void GpsGorev()
{
	GPIOD->ODR ^= GPIO_PIN_12;
	GpsPaket.GpsPaketOlustur(*gps.LatitudeAl(),*gps.LongitudeAl(),*barometre.IrtifaOku(0),*barometre.SicaklikOku());
	GpsPaket.gpsPaketCagir(GpsDataPacket);
	ImuPaket.ImuPaketOlustur(*imu.PitchAl(), *imu.RollAl(), *mag.HeadingOlustur(pitch,roll), *imu.SicaklikAl());
	ImuPaket.imuPaketCagir(ImuDataPacket);
	if(ArayuzPaket.YoklamaFlag)
	{
		HAL_UART_Transmit(&huart3, GpsDataPacket, sizeof(GpsDataPacket), 1000);
		HAL_UART_Transmit(&huart3, ImuDataPacket, sizeof(ImuDataPacket), 1000);
	}
}
void ImuGorev()
{
	GPIOD->ODR ^= GPIO_PIN_13;
	imu.DBC_ACI_BULMA();

	pitch=*imu.PitchAl();
	roll=*imu.RollAl();
	yaw=*imu.YawAl();
	heading =*mag.HeadingOlustur(pitch,roll);
	irtifa=*barometre.IrtifaOku(0);
}

void YonelmeGorev()
{
    ArabaDurumlar Durum= MesafeBul;
    if(ArayuzPaket.GidilecekNoktaBayrak)
    {
        hedefbayrak = true;
        while(hedefbayrak)
        {
            switch(Durum)
            {
                case MesafeBul:
                    gidilecekMesafe_f = araba.mesafeBul(*gps.LatitudeAl(), *gps.LongitudeAl(), *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLonAl());
                    Durum = YonelimBul;
                    break;

                case YonelimBul:
                    donulecekAci_f = araba.yonelimBul(*gps.LatitudeAl(), *gps.LongitudeAl(), *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLonAl());
                    Durum = Yonel;

                    break;

                case Yonel:
                	if(gidilecekMesafe_f>=10)
                	{
                		if(heading < donulecekAci_f - 4 || heading > donulecekAci_f + 4)
                		{
                			hata = donulecekAci_f - heading;
                			if(hata > 180)
                			{
                				hata = hata - 360;
                			}
                			else if(hata < -180)
                			{
                				hata = 360 + hata;
                			}

                			if(abs(hata) < 10)
                			{
                				integral += hata;
                			}
                			else
                			{
                				integral = 0;
							}

							kontrolSinyali = Kp * hata + Ki * integral;
							if(kontrolSinyali > 1000 || kontrolSinyali < -1000)
							{
								kontrolSinyali = 1000;
							}

							if(hata < 0)
							{
								motorSol.PWM(abs(kontrolSinyali), TIM_CHANNEL_4);
								solaDonbayrak=true;
							}
							else
							{
								motorSag.PWM(abs(kontrolSinyali), TIM_CHANNEL_2);
								sagaDonbayrak=true;
							}

							heading = *mag.HeadingOlustur(pitch,roll);  // heading güncelleniyor
						}
						else
						{
							 motorSag.PWM(1000, TIM_CHANNEL_2);
							 motorSol.PWM(1000, TIM_CHANNEL_4);
							 araba.duzGit();
						}
                	}
                	hedefbayrak = false;
                	break;
            }
        }

        if(gidilecekMesafe_f < 10)
        {
            ArayuzPaket.GidilecekNoktaBayrak = false;
            araba.Dur();
        }
    }
}
void ArayuzTask()
{
	 if(ArayuzPaket.PaketCozBayrak)
	 {
		 ArayuzPaket.PaketCoz();
		 ArayuzPaket.PaketCozBayrak = false;
	 }
}

extern "C" void TIM3_IRQHandler()
{
	gorev.GorevBayrakKaldir();
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
