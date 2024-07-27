/*
 * Barometre.cpp
 *
 *  Created on: Jul 27, 2024
 *      Author: onurd
 */
#include "Barometre.h"
#include <math.h>

Barometre::Barometre(I2C_HandleTypeDef* hi2c, uint8_t address)
{
    this->hi2c=hi2c;
	this->address=address;
	T=0;
	P=0;
}

void Barometre::Yapilandir()
{
	HAL_I2C_Mem_Read(hi2c, address, 0xAA, 1, calibDatas, 22, 100);
	AC1 = (calibDatas[0] << 8) | calibDatas[1];
	AC2 = (calibDatas[2] << 8) | calibDatas[3];
	AC3 = (calibDatas[4] << 8) | calibDatas[5];
	AC4 = (calibDatas[6] << 8) | calibDatas[7];
	AC5 = (calibDatas[8] << 8) | calibDatas[9];
	AC6 =(calibDatas[10] << 8) | calibDatas[11];
	B1 = (calibDatas[12] << 8) | calibDatas[13];
	B2 = (calibDatas[14] << 8) | calibDatas[15];
	MB = (calibDatas[16] << 8) | calibDatas[17];
	MC = (calibDatas[18] << 8) | calibDatas[19];
	MD = (calibDatas[20] << 8) | calibDatas[21];
}

float Barometre::SicaklikOku()
{
	UT = regSicaklikOku();
	X1 = (UT - AC6) * AC5 / pow(2, 15);
	X2 = (MC * pow(2, 11)) / (X1 + MD);
	B5 = X1 + X2;
	T = (B5 + 8) / pow(2, 4);
    return T / 10.0;
}

float Barometre::BasincOku(uint8_t oss)
{
    BasincHesapla(oss);
    return P;
}

float Barometre::IrtifaOku(uint8_t oss)
{
    BasincOku(oss);
    return 44330 * (1 - (pow((P / Po), 1 / 5.255)));
}

void Barometre::BasincHesapla(uint8_t oss)
{
    UP = regBasincOku(oss);
    B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / pow(2, 12))) / pow(2, 11);
    X2 = AC2 * B6 / pow(2, 11);
    X3 = X1 + X2;
    B3 = (((AC1 * 4 + X3) << oss) + 2) / 4;
    X1 = AC3 * B6 / pow(2, 13);
    X2 = (B1 * (B6 * B6 / pow(2, 12))) / pow(2, 16);
    X3 = ((X1 + X2) + 2) / pow(2, 2);
    B4 = AC4 * (unsigned long)(X3 + 32768) / pow(2, 15);
    B7 = ((unsigned long)UP - B3) * (50000 >> oss);
    P = (B7 < 0x80000000) ? (B7 * 2) / B4 : (B7 / B4) * 2;
    X1 = (P / pow(2, 8)) * (P / pow(2, 8));
    X1 = (X1 * 3038) / pow(2, 16);
    X2 = (-7357 * P) / pow(2, 16);
    P = P + (X1 + X2 + 3791) / pow(2, 4);
}

uint16_t Barometre::regSicaklikOku()
{
    data = 0x2E;
    uint8_t tempRAW[2] = {0};

    writeAndRead(tempRAW, data, 'T', 4);
    return ((tempRAW[0] << 8) | tempRAW[1]);
}

uint16_t Barometre::regBasincOku(uint8_t oss)
{
    data = 0x34 + (oss << 6);
    uint8_t pressRaw[3] = {0};

    writeAndRead(pressRaw, data, 'P', oss);
    return (((pressRaw[0] << 16) + (pressRaw[1] << 8) + pressRaw[2]) >> (8 - oss));
}

void Barometre::writeAndRead(uint8_t* raw, uint8_t data, char sens, uint8_t oss)
{
    uint8_t size;
    HAL_I2C_Mem_Write(hi2c, address, 0xF4, 1, &data, 1, 100);

    if (sens == 'P')
    {
        size = 3;
        if (oss == 0) HAL_Delay(5);
        else if (oss == 1) HAL_Delay(8);
        else if (oss == 2) HAL_Delay(14);
        else if (oss == 3) HAL_Delay(26);
    }
    else
    {
        size = 2;
        HAL_Delay(5);
    }

    HAL_I2C_Mem_Read(hi2c, address, 0xF6, 1, raw, size, 100);
}
