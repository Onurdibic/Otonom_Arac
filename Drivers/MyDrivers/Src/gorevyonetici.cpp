#include "gorevyonetici.h"


Gorevyonetici::Gorevyonetici(TIM_TypeDef* pTimerAlan)
{
    this->pTimerAlan = pTimerAlan;
}

void Gorevyonetici::GorevAl(void (*gorev)(), int16_t ms_s16)
{
	if (gorevSayac < maxGorev)
	{
	    gorevler[gorevSayac].gorev = gorev;
	    gorevler[gorevSayac].ms_s16 = ms_s16;
	    gorevSayac++;
	}
}
void Gorevyonetici::BayrakKaldir()
{
    pTimerAlan->SR &= ~(TIM_SR_UIF);
    Counter_u16++;
    for (int i = 0; i < gorevSayac; i++)
    {
		if (0 == Counter_u16 % gorevler[i].ms_s16)
		{
		   gorevler[i].Bayrak = true;
		}
     }
    if (Counter_u16 % 1000 == 0)
  	{
  	    Counter_u16=0;
  	}
}

void Gorevyonetici::GorevCalistir()
{

	for (int i = 0; i < gorevSayac; i++)
	{
		if (gorevler[i].Bayrak && gorevler[i].gorev)
	    {
	        gorevler[i].Bayrak = false;
	        gorevler[i].gorev();
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
