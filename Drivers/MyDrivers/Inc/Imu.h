/*
 * Imu.h
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */

#ifndef MYOTONOMDRIVERS_INC_IMU_H_
#define MYOTONOMDRIVERS_INC_IMU_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"
#include <cmath>

class MyImu {
public:
    MyImu(I2C_HandleTypeDef *hi2c1);
    void DBC_MPU6500_Reset();
    void DBC_MPU6500_YAPILANDIR();
    void DBC_ACC_OKU();
    void DBC_GYRO_OKU();
    void DBC_SICAKLIK_OKU();
    void DBC_GYRO_OFSET();
    void DBC_DATA_OKU();
    void DBC_ACI_BULMA();
    float* PitchAl();
    float* RollAl();
    float* YawAl();
    float* SicaklikAl();
private:
    I2C_HandleTypeDef *hi2c;
    uint8_t data_u8;
    int16_t gyroEksen[3], accEksen[3];
    float gyroHesap[3];
    int16_t accToplamVektor_s16;
    float accPitchAci_f, accRollAci_f;
    float gyroRollAci_f, gyroPitchAci_f, gyroYawAci_f;
    float rollAci_f, pitchAcisi_f;
    uint16_t hamSicaklik_u16;
    float Sicaklik_f;

};


#endif /* MYOTONOMDRIVERS_INC_IMU_H_ */
