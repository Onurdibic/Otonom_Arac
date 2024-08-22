/*
 * Barometre.cpp
 *
 *  Created on: Jul 27, 2024
 *      Author: onurd
 */
#include "Barometre.h"
#include <math.h>

Barometre::Barometre(I2C_HandleTypeDef* hi2c)
{
    this->hi2c=hi2c;
	this->adres=0xEE;
	sicaklik_f=0;
	basinc_f=0;
}

void Barometre::Yapilandir()
{
	HAL_I2C_Mem_Read(hi2c, adres, 0xAA, 1, hamDatalar, 22, 100);
	AC1 = (hamDatalar[0] << 8) | hamDatalar[1];
	AC2 = (hamDatalar[2] << 8) | hamDatalar[3];
	AC3 = (hamDatalar[4] << 8) | hamDatalar[5];
	AC4 = (hamDatalar[6] << 8) | hamDatalar[7];
	AC5 = (hamDatalar[8] << 8) | hamDatalar[9];
	AC6 =(hamDatalar[10] << 8) | hamDatalar[11];
	B1 = (hamDatalar[12] << 8) | hamDatalar[13];
	B2 = (hamDatalar[14] << 8) | hamDatalar[15];
	MB = (hamDatalar[16] << 8) | hamDatalar[17];
	MC = (hamDatalar[18] << 8) | hamDatalar[19];
	MD = (hamDatalar[20] << 8) | hamDatalar[21];
}

float *Barometre::SicaklikOku()
{
	 UT = regSicaklikOku();
	 X1 = (UT - AC6) * AC5 >> 15;
	 X2 = (MC << 11) / (X1 + MD);
	 B5 = X1 + X2;
	 sicaklik_f = ((B5 + 8) >> 4) / 10.0;

    return &sicaklik_f ;
}

float Barometre::BasincOku(uint8_t oss)
{
	 UP = regBasincOku(oss);
	 B6 = B5 - 4000;
	 X1 = (B2 * ((B6 * B6) >> 12)) >> 11;
	 X2 = (AC2 * B6) >> 11;
	 X3 = X1 + X2;
	 B3 = (((AC1 * 4 + X3) << oss) + 2) >> 2;
	 X1 = (AC3 * B6) >> 13;
	 X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
	 X3 = ((X1 + X2) + 2) >> 2;
	 B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;
	 B7 = ((uint32_t)UP - B3) * (50000 >> oss);
	 basinc_f = (B7 < 0x80000000) ? (B7 * 2) / B4 : (B7 / B4) * 2;
	 X1 = (basinc_f / 256.0) * (basinc_f / 256.0);
	 X1 = (X1 * 3038) >> 16;
	 X2 = (-7357 *basinc_f)  / 65536.0;
	 basinc_f += (X1 + X2 + 3791) >> 4;
	 return basinc_f;
}

float *Barometre::IrtifaOku(uint8_t oss)
{
    BasincOku(oss);
    irtifa_f = (44330.0 * (1.0 - pow((basinc_f / 101325.0), 0.1903)));
    return &irtifa_f;
}

uint16_t Barometre::regSicaklikOku()
{
    data = 0x2E;
    uint8_t hamSicaklik[2] = {0};

    writeAndRead(hamSicaklik, data, 'S', 4);
    return ((hamSicaklik[0] << 8) | hamSicaklik[1]);
}

uint16_t Barometre::regBasincOku(uint8_t oss)
{
    data = 0x34 + (oss << 6);
    uint8_t hamBasinc[3] = {0};

    writeAndRead(hamBasinc, data, 'B', oss);
    return (((hamBasinc[0] << 16) + (hamBasinc[1] << 8) + hamBasinc[2]) >> (8 - oss));
}

void Barometre::writeAndRead(uint8_t* hamData_u8, uint8_t data, char secim, uint8_t oss)
{
    uint8_t boyut;
    HAL_I2C_Mem_Write(hi2c, adres, 0xF4, 1, &data, 1, 100);

    if (secim == 'B')
    {
    	boyut = 3;
        if (oss == 0) HAL_Delay(5);
        else if (oss == 1) HAL_Delay(8);
        else if (oss == 2) HAL_Delay(14);
        else if (oss == 3) HAL_Delay(26);
    }
    else
    {
    	boyut = 2;
        HAL_Delay(5);
    }

    HAL_I2C_Mem_Read(hi2c, adres, 0xF6, 1, hamData_u8, boyut, 100);
}
