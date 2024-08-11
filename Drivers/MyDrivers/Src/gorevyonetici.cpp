#include "gorevyonetici.h"

Gorevyonetici::Gorevyonetici(TIM_TypeDef* pTimerAlan)
{
    this->pTimerAlan = pTimerAlan;
}

Gorev::Gorev(){}

void Gorevyonetici::GorevAl(void (*gorev)(), int16_t ms_s16)
{
	if (gorevSayac_u8 < maxGorev)
	{
		gorevler[gorevSayac_u8].GorevGir(gorev, ms_s16);
		gorevSayac_u8++;
	}
}
void Gorevyonetici::GorevBayrakKaldir()
{
    pTimerAlan->SR &= ~(TIM_SR_UIF);//kesme bayrağı sıfırla
    Counter_u16++;
    for (int i = 0; i < gorevSayac_u8; i++)
    {
		if (0 == Counter_u16 % gorevler[i].MsAl())
		{
			gorevler[i].BayrakDuzenle(true);
		}
     }
    if (Counter_u16 % 5000 == 0)
  	{
  	    Counter_u16=0;
  	}
}

void Gorevyonetici::GorevCalistir()
{

	for (int i = 0; i < gorevSayac_u8; i++)
	{
		if (gorevler[i].Bayrak)
	    {
			gorevler[i].BayrakDuzenle(false);
			gorevler[i].Calistir();
	    }
	}

}
void Gorevyonetici::IRQHandler_()
{
	 taskCounter_u16++;

	    if (taskCounter_u16 % 10==0)
	    {
	    	Task100HzBayrak=true;
	    }
	    if (taskCounter_u16 % 20==0)
	    {
	    	Task50HzBayrak=true;
	    }
	    if (taskCounter_u16 % 50==0)
	    {
	    	Task20HzBayrak=true;
	    }
	    if (taskCounter_u16 % 200==0)
	    {
	    	Task5HzBayrak=true;
	    }
	    if (taskCounter_u16 % 1000 == 0)
	    {
	        Task1HzBayrak=true;
	        taskCounter_u16=0;
	    }

}

void Gorev::GorevGir(void (*gorev)(), int16_t ms_s16)
{
    this->gorev = gorev;
    this->ms_s16 = ms_s16;
}

void Gorev::Calistir()
{
    if (gorev)
    {
        gorev();
    }
}

void Gorev::BayrakDuzenle(bool value){Bayrak = value;}

int16_t Gorev::MsAl(){return ms_s16;}
