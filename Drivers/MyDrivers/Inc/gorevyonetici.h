/*
 * gorevyonetici.h
 *
 *  Created on: Jul 3, 2024
 *      Author: onurd
 */
#ifndef MYDRIVERS_INC_GOREVYONETICI_H_
#define MYDRIVERS_INC_GOREVYONETICI_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"

#define maxGorev 10

struct Gorev {
    void (*gorev)();
    int16_t ms_s16;
    bool Bayrak=false;
};

class Gorevyonetici {
public:
    Gorevyonetici(TIM_TypeDef* pTimerAlan);

    void GorevAl(void (*gorev)(), int16_t ms_s16);
    void BayrakKaldir();
    void GorevCalistir();
    void IRQHandler_();

    bool Task1HzBayrak=false;
    bool Task5HzBayrak=false;
    bool Task20HzBayrak=false;
    bool Task50HzBayrak=false;
    bool Task100HzBayrak=false;
private:
    TIM_TypeDef* pTimerAlan;

    void (*gorev)();
    Gorev gorevler[maxGorev];
    uint8_t gorevSayac = 0;
    uint16_t taskCounter_u16 = 0;
    uint16_t Counter_u16=0;

};

#endif /* MYDRIVERS_INC_GOREVYONETICI_H_ */
