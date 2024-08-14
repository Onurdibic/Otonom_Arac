/*
 * program.cpp
 *
 *  Created on: Jul 14, 2024
 *      Author: onurd
 */
#include "program.h"
#include "usart.h"
#include <cmath>
#include "tim.h"
#include "i2c.h"

#include "Nesneler.h"

/************************Degiskenler************************/
float Kp = 12;
float Ki = 1;
float hata = 0;
float kontrolSinyali1_f;
float kontrolSinyali2_f;
float integral = 0;
float aci;
int8_t tur;
uint8_t GpsVeriPaket[21]={0};
uint8_t ImuVeriPaket[21]={0};
uint8_t VersiyonVeriPaket[8]={0};
uint8_t YoklamaVeriPaket[8]={0};
uint8_t RotaVeriPaket[8]={0};
float pitch;
float roll;
float yaw;
float heading;
float irtifa;
float imusicaklik;
float barosicaklik;
float gidilecekMesafe_f=0;
float donulecekAci_f =0;
float baslangicHatasi=0;
float baslangicHeading=0;
float pwmOffset=450;
bool yonelmeBayrak=false;
bool hedefbayrak=true;
bool solaDonbayrak=false;
bool sagaDonbayrak=false;
enum ArabaDurumlar
{
	Hazirlik,
	Yonelim
};
/*****************Fonksiyon Bildirimleri********************/
void GpsVeriGorev();
void EksenGorev();
void YonelmeGorev();
void ImuVeriGorev();
void ArayuzTask();
void YonelmeBayrakGorev();
void setup()
{
	//uart3.Yapilandir(115200, GPIOD, GPIO_PIN_8,GPIOB ,GPIO_PIN_11);
	//uart3.receiveIT(RotaData, 8);

	ArayuzPaket.PaketKesmeYapilandir();
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
	gorev.GorevAl(EksenGorev,30);
	gorev.GorevAl(YonelmeGorev,35);
	gorev.GorevAl(ArayuzTask,100);
	gorev.GorevAl(ImuVeriGorev,700);
	gorev.GorevAl(GpsVeriGorev,1000);
	gorev.GorevAl(YonelmeBayrakGorev,5000);
	motorSag.PWM(400,TIM_CHANNEL_2);
	motorSol.PWM(400,TIM_CHANNEL_4);
	araba.Dur();

}

void loop()
{
	//motor1.AciBul();
	//tur=*motor1.TurAl();
	//aci=*motor1.AciAl();

	gorev.GorevCalistir();

}

void GpsVeriGorev()
{
	GPIOD->ODR ^= GPIO_PIN_12;
	irtifa=*barometre.IrtifaOku(0); // baro irtifa al
	GpsPaket.GpsPaketOlustur(41.2174316, 36.4566603,irtifa,barosicaklik);
	GpsPaket.gpsPaketCagir(GpsVeriPaket);
	ImuPaket.ImuPaketOlustur(pitch, roll,heading, imusicaklik);
	ImuPaket.imuPaketCagir(ImuVeriPaket);
	if(ArayuzPaket.YoklamaFlag)
	{
		HAL_UART_Transmit(&huart3, GpsVeriPaket, sizeof(GpsVeriPaket), 1000);
	}
}
void EksenGorev()
{
	GPIOD->ODR ^= GPIO_PIN_13;
	imu.DBC_ACI_BULMA(); //imu pitch roll yaw al

	pitch=*imu.PitchAl();
	roll=*imu.RollAl();
	yaw=*imu.YawAl();
	heading =*mag.HeadingOlustur(pitch,roll); // mag heading al
	imusicaklik=*imu.SicaklikAl();
	barosicaklik=*barometre.SicaklikOku();
}
void ImuVeriGorev()
{
	if(ArayuzPaket.YoklamaFlag)
	{
		HAL_UART_Transmit(&huart3, ImuVeriPaket, sizeof(ImuVeriPaket), 1000);
	}
}
void YonelmeGorev()
{
    ArabaDurumlar Durum= Hazirlik;
    if(ArayuzPaket.GidilecekNoktaBayrak==true && yonelmeBayrak==true && ArayuzPaket.arabaDurBayrak==false)
    {
        hedefbayrak = true;
        while(hedefbayrak)
        {
            switch(Durum)
            {
                case Hazirlik:
                    gidilecekMesafe_f = araba.mesafeBul(41.2174316,36.4566603, *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLonAl());
                    donulecekAci_f = araba.yonelimBul(41.2174316, 36.4566603, *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLonAl());
                    Durum = Yonelim;
                    break;

                case Yonelim:
                	if(gidilecekMesafe_f>=7)
                	{
                		if(heading < donulecekAci_f - 3 || heading > donulecekAci_f + 3)
                		{
                			hata = donulecekAci_f - heading;
                			integral += abs(hata);
                			if(hata > 180)
                			{
                				hata = hata - 360;
                			}
                			else if(hata < -180)
                			{
                				hata = 360 + hata;
                			}

                			integral=isaret.ustSinirla(integral,200);

                			kontrolSinyali1_f = pwmOffset+Kp * abs(hata) + Ki * integral;
                			kontrolSinyali1_f=isaret.ustSinirla(kontrolSinyali1_f, 1000);
                			kontrolSinyali1_f=isaret.altSinirla(kontrolSinyali1_f, 450);
                			kontrolSinyali2_f = 300 - ((kontrolSinyali1_f - 450) * (300 - 75)) / (1000 - 450);
                			kontrolSinyali2_f=isaret.ustSinirla(kontrolSinyali2_f, 300);
                			kontrolSinyali2_f=isaret.altSinirla(kontrolSinyali2_f, 75);
                			if(hata < 0)
                			{
                				motorSag.PWM(kontrolSinyali1_f, TIM_CHANNEL_2);
                				motorSol.PWM(kontrolSinyali2_f, TIM_CHANNEL_4);
                				araba.solGit();
                			}
                			else
                			{
                				motorSol.PWM(kontrolSinyali1_f, TIM_CHANNEL_4);
                				motorSag.PWM(kontrolSinyali2_f, TIM_CHANNEL_2);
                				araba.sagGit();
                			}
                			heading =*mag.HeadingOlustur(pitch,roll); // mag heading al
                		}
                		else
                		{
                			motorSag.PWM(1000, TIM_CHANNEL_2);
                			motorSol.PWM(1000, TIM_CHANNEL_4);
                			araba.Dur();
                			integral=0;
                			yonelmeBayrak=false;
                		}
                	}
                	hedefbayrak = false;

                	break;
            }
        }
        if(gidilecekMesafe_f < 7)
        {
            ArayuzPaket.GidilecekNoktaBayrak = false;

            araba.Dur();
        }
    }
    if(ArayuzPaket.arabaDurBayrak==true)
    {
    	araba.Dur();
    	ArayuzPaket.GidilecekNoktaBayrak=false;
    	ArayuzPaket.arabaDurBayrak=false;
    }
}

void YonelmeBayrakGorev()
{
	yonelmeBayrak=true;
}
void ArayuzTask()
{
	 if(ArayuzPaket.PaketCozBayrak)
	 {
		 ArayuzPaket.PaketCozBayrak = false;
		 ArayuzPaket.PaketCoz();
		 if(ArayuzPaket.VersiyonPaketBayrak==true)
		 {
			 VersiyonPaket.VersiyonPaketOlustur(0, 0, 9);
			 VersiyonPaket.versiyonPaketCagir(VersiyonVeriPaket);
			 HAL_UART_Transmit(&huart3, VersiyonVeriPaket, sizeof(VersiyonVeriPaket), 1000);
			 ArayuzPaket.VersiyonPaketBayrak=false;
		 }
		 if(ArayuzPaket.YoklamaPaketFlag==true)
		 {
			 YoklamaPaket.YoklamaPaketOlustur();
			 YoklamaPaket.yoklamaPaketCagir(YoklamaVeriPaket);
			 HAL_UART_Transmit(&huart3, YoklamaVeriPaket, sizeof(YoklamaVeriPaket), 1000);
			 ArayuzPaket.YoklamaPaketFlag=false;
		 }
	 }
	 if(ArayuzPaket.GidilecekNoktaBayrak==false && ArayuzPaket.RotaGeldiBayrak==true)
	 {
		 RotaPaket.RotaPaketOlustur();
		 RotaPaket.rotaPaketCagir(RotaVeriPaket);
		 HAL_UART_Transmit(&huart3, RotaVeriPaket, sizeof(RotaVeriPaket), 1000);
		 ArayuzPaket.RotaGeldiBayrak=false;
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
		ArayuzPaket.ArayuzDataAlveBayrakKaldir();
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
