#include "mytimer.h"

Timer::Timer(TIM_TypeDef* pTimerAlan)
{
    this->pTimerAlan = pTimerAlan;
    TimerClockAktifEt();
}

void Timer::Yapilandir(uint32_t prescaler_u32, uint32_t period_u32)
{
	pTimerAlan->PSC = prescaler_u32 - 1;
	pTimerAlan->ARR = period_u32 - 1;
	pTimerAlan->DIER |= TIM_DIER_UIE;
    TimerInterruptAktifEt();
}

void Timer::AktifEt()
{
	pTimerAlan->CR1 |= TIM_CR1_CEN;
}

void Timer::Kapat()
{
	pTimerAlan->CR1 &= ~TIM_CR1_CEN;
}

uint16_t Timer::SayacDegerAl()
{
    return pTimerAlan->CNT;
}

void Timer::TimerClockAktifEt()
{
    if (this->pTimerAlan == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
    else if (this->pTimerAlan == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    else if (this->pTimerAlan == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
    else if (this->pTimerAlan == TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
    else if (this->pTimerAlan == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
    }
    else if (this->pTimerAlan == TIM7)
    {
        __HAL_RCC_TIM7_CLK_ENABLE();
    }
}

void Timer::TimerInterruptAktifEt()
{
    if (this->pTimerAlan == TIM2)
    {
        NVIC_EnableIRQ(TIM2_IRQn);
        NVIC_SetPriority(TIM2_IRQn, 2);
    }
    else if (this->pTimerAlan == TIM3)
    {
        NVIC_EnableIRQ(TIM3_IRQn);
        NVIC_SetPriority(TIM3_IRQn, 2);
    }
    else if (this->pTimerAlan == TIM4)
    {
        NVIC_EnableIRQ(TIM4_IRQn);
        NVIC_SetPriority(TIM4_IRQn, 2);
    }
    else if (this->pTimerAlan == TIM5)
    {
        NVIC_EnableIRQ(TIM5_IRQn);
        NVIC_SetPriority(TIM5_IRQn, 2);
    }
    else if (this->pTimerAlan == TIM6)
    {
        NVIC_EnableIRQ(TIM6_DAC_IRQn);
        NVIC_SetPriority(TIM6_DAC_IRQn, 2);
    }
    else if (this->pTimerAlan == TIM7)
    {
        NVIC_EnableIRQ(TIM7_IRQn);
        NVIC_SetPriority(TIM7_IRQn, 2);
    }
}

