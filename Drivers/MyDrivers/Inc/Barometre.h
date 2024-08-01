/*
 * Barometre.h
 *
 *  Created on: Jul 27, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_BAROMETRE_H_
#define MYDRIVERS_INC_BAROMETRE_H_

#include <math.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>

class Barometre {
public:
    Barometre(I2C_HandleTypeDef* i2c_handle, uint8_t address);
    void Yapilandir();
    float *SicaklikOku();
    float BasincOku(uint8_t oss);
    float *IrtifaOku(uint8_t oss);

private:
    I2C_HandleTypeDef* hi2c;
    void KalibrasyonDatalariOku();
    uint16_t regSicaklikOku();
    uint16_t regBasincOku(uint8_t oss);
    void writeAndRead(uint8_t* raw, uint8_t data, char sens, uint8_t oss);

    uint8_t address;
    uint8_t data;
    uint8_t calibDatas[22]={0};
    float Po = 101325;
    float irtifa_f=0;
    // Calibration data
    short AC1, AC2, AC3;
    unsigned short AC4, AC5, AC6;
    short B1, B2, MB, MC, MD;

    // Calculated values
    int32_t UT, UP, X1, X2, X3, B3, B5, B6;

    unsigned long B4, B7;
    float T, P;
};

#endif /* MYDRIVERS_INC_BAROMETRE_H_ */
