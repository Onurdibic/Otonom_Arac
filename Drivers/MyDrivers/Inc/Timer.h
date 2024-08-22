/*
 * Timer.h
 *
 *  Created on: Aug 22, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_TIMER_H_
#define MYDRIVERS_INC_TIMER_H_


#include "stm32f4xx_hal.h"
#include "stdint.h"

class Timer {
public:
	Timer(TIM_TypeDef* pTimerAlan);

    void Yapilandir(uint32_t prescaler_u32, uint32_t period_u32); //Yapilandir
    void AktifEt();
    void Kapat();
    uint16_t SayacDegerAl();

private:
    TIM_TypeDef* pTimerAlan;

    void TimerClockAktifEt();
    void TimerInterruptAktifEt();

};

#endif /* MYDRIVERS_INC_TIMER_H_ */
