/*
 * program.cpp
 *
 *  Created on: Jul 14, 2024
 *      Author: onurd
 */
#include "program.h"
#include "Nesneler.h"
#include <cmath>
/************************Degiskenler************************/
float KpCizgisel = 0.4;
float KiCizgisel = 0.001;
float KpAcisal = 0.035;
float KiAcisal = 0.005;
float mesafeIntegral = 0.0;
float cizgiselintegral=0.0;
float gidilecekMesafe_f=0;
float donulecekAci_f =0;
float hata_f = 0;
float kontrolSinyali1_f;
float kontrolSinyali2_f;
float yaricap_f;
float kapanacakAci_f;
float acisalHiz_f;
float cizgiselHiz_f;
float toplamPWM_f;
float pwmsol_f;
float pwmsag_f;
int8_t tur;
uint8_t GpsVeriPaket[17]={0};
uint8_t ImuVeriPaket[21]={0};
uint8_t SistemVeriPaket[13]={0};
uint8_t VersiyonVeriPaket[8]={0};
uint8_t YoklamaVeriPaket[8]={0};
uint8_t RotaVeriPaket[8]={0};
float batarya=0;
float voltaj;
float pitch;
float roll;
float yaw;
float heading;
float irtifa;
float imusicaklik;
float barosicaklik;

/*****************Fonksiyon Bildirimleri********************/
void GpsVeriGorev();
void EksenGorev();
void ImuVeriGorev();
void AdcOkuGorev();
void ArayuzGorev();
void HedefGorev();
void setup()
{
	vsensor1.Yapilandir();
	ArayuzPaket.PaketKesmeYapilandir();
	motorSag.Yapilandir();
	motorSol.Yapilandir();
	gps.Yapilandir();
	mag.Yapilandir();
	imu.DBC_MPU6500_YAPILANDIR();
	barometre.Yapilandir();

	HAL_Delay(1000);
	GPIOD->ODR ^= GPIO_PIN_12;
	mag.KalibreEt();
	GPIOD->ODR ^= GPIO_PIN_12;
	HAL_Delay(2000);
	imu.DBC_GYRO_OFSET();
	gorev.GorevAl(EksenGorev,30);
	gorev.GorevAl(HedefGorev,50);
	gorev.GorevAl(ArayuzGorev,100);
	gorev.GorevAl(ImuVeriGorev,700);
	gorev.GorevAl(GpsVeriGorev,1000);

	araba.Dur();
	timer3.Yapilandir(82500,5);
	timer3.AktifEt();

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

	if(ArayuzPaket.YoklamaFlag)
	{
		GpsPaket.GpsPaketOlustur(*gps.LatitudeAl(),*gps.LongitudeAl(),irtifa,barosicaklik);
		GpsPaket.gpsPaketCagir(GpsVeriPaket);
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
	imusicaklik=*imu.SicaklikAl();
	heading =*mag.HeadingOlustur(pitch,roll); // mag heading al
	barosicaklik=*barometre.SicaklikOku();
	irtifa=*barometre.IrtifaOku(0);
}
void ImuVeriGorev()
{
	if(ArayuzPaket.YoklamaFlag)
	{
		ImuPaket.ImuPaketOlustur(pitch, roll,heading, imusicaklik);
		ImuPaket.imuPaketCagir(ImuVeriPaket);
		HAL_UART_Transmit(&huart3, ImuVeriPaket, sizeof(ImuVeriPaket), 1000);
	}
}

void HedefGorev()
{
	if (ArayuzPaket.GidilecekNoktaBayrak == true  && ArayuzPaket.arabaDurBayrak == false)
	{
		gidilecekMesafe_f = araba.mesafeBul(*gps.LatitudeAl(),*gps.LongitudeAl(), *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLonAl());
		donulecekAci_f = araba.yonelimBul(*gps.LatitudeAl(),*gps.LongitudeAl(), *ArayuzPaket.ArayuzLatAl(), *ArayuzPaket.ArayuzLonAl());

		hata_f = donulecekAci_f - heading;

		if (hata_f > 180)
		{
			hata_f = hata_f - 360;
		}
		else if (hata_f < -180)
		{
			hata_f = 360 + hata_f;
		}

		mesafeIntegral += gidilecekMesafe_f;
		mesafeIntegral = isaret.ustSinirla(mesafeIntegral, 100);

		cizgiselHiz_f = KpCizgisel * gidilecekMesafe_f + KiCizgisel * mesafeIntegral;
		cizgiselHiz_f = isaret.ustSinirla(cizgiselHiz_f, 0.85);
		cizgiselHiz_f = isaret.altSinirla(cizgiselHiz_f, 0.3);

		cizgiselintegral += abs(hata_f);
		cizgiselintegral = isaret.ustSinirla(cizgiselintegral, 150);

		kapanacakAci_f = KpAcisal * abs(hata_f) + KiAcisal * abs(cizgiselintegral);
		float radyan = kapanacakAci_f * (M_PI / 180.0);
		acisalHiz_f = radyan / 0.05;
		yaricap_f = cizgiselHiz_f / acisalHiz_f;

		if (abs(hata_f) > 3)
		{
			if (hata_f < 0)
			{
				pwmsol_f = cizgiselHiz_f - (acisalHiz_f / 2);
				pwmsag_f = cizgiselHiz_f + (acisalHiz_f / 2);
				toplamPWM_f= 2000 * (cizgiselHiz_f / 1.22);
				float toplam = pwmsol_f + pwmsag_f;

				kontrolSinyali1_f = toplamPWM_f * (pwmsag_f / toplam);
				kontrolSinyali2_f = toplamPWM_f * (pwmsol_f / toplam);
				kontrolSinyali1_f=isaret.ustSinirla(kontrolSinyali1_f, 1000);
				kontrolSinyali1_f=isaret.altSinirla(kontrolSinyali1_f, (toplamPWM_f/2)+cizgiselHiz_f*125);
				kontrolSinyali2_f=isaret.ustSinirla(kontrolSinyali2_f, (toplamPWM_f/2)-cizgiselHiz_f*125);
				kontrolSinyali2_f=isaret.altSinirla(kontrolSinyali2_f, pow(cizgiselHiz_f, 5) * 910);
				motorSag.PWM(kontrolSinyali1_f, TIM_CHANNEL_2);
				motorSol.PWM(kontrolSinyali2_f, TIM_CHANNEL_4);
				araba.solGit();
			}
			else
			{
				pwmsol_f = cizgiselHiz_f + (acisalHiz_f / 2);
				pwmsag_f = cizgiselHiz_f - (acisalHiz_f / 2);
				toplamPWM_f= 2000 * (cizgiselHiz_f / 1.22);
				float toplam = pwmsol_f + pwmsag_f;

				kontrolSinyali1_f = toplamPWM_f * (pwmsol_f / toplam);
				kontrolSinyali2_f = toplamPWM_f * (pwmsag_f / toplam);
				kontrolSinyali1_f=isaret.ustSinirla(kontrolSinyali1_f, 1000);
				kontrolSinyali1_f=isaret.altSinirla(kontrolSinyali1_f, (toplamPWM_f/2)+cizgiselHiz_f*130);
				kontrolSinyali2_f=isaret.ustSinirla(kontrolSinyali2_f, (toplamPWM_f/2)-cizgiselHiz_f*130);
				kontrolSinyali2_f=isaret.altSinirla(kontrolSinyali2_f,  pow(cizgiselHiz_f, 5) * 910);
				motorSol.PWM(kontrolSinyali1_f, TIM_CHANNEL_4);
				motorSag.PWM(kontrolSinyali2_f, TIM_CHANNEL_2);
				araba.sagGit();
			}
		}
		else
		{
			toplamPWM_f= 2000 * (cizgiselHiz_f / 1.22);
			motorSag.PWM(toplamPWM_f/2, TIM_CHANNEL_2);
			motorSol.PWM(toplamPWM_f/2, TIM_CHANNEL_4);
			araba.duzGit();
			cizgiselintegral = 0;
		}
	}

	if (cizgiselHiz_f <= 0.35)
	{
		ArayuzPaket.GidilecekNoktaBayrak = false;
		araba.Dur();
	}
	if (ArayuzPaket.arabaDurBayrak == true)
	{
		araba.Dur();
		ArayuzPaket.GidilecekNoktaBayrak = false;
		ArayuzPaket.arabaDurBayrak = false;
	}
}

void ArayuzGorev()
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
void AdcOkuGorev()
{
	voltaj=vsensor1.Olc();
	batarya=vsensor1.BataryaYuzdeBul();
	if(ArayuzPaket.YoklamaFlag)
	{
		SistemPaket.SistemPaketOlustur(barosicaklik,batarya);
		SistemPaket.sistemPaketCagir(SistemVeriPaket);
		HAL_UART_Transmit(&huart3, SistemVeriPaket, sizeof(SistemVeriPaket), 1000);
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
