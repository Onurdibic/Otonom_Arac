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
    void DBC_READ_DATA();
    void DBC_ACI_BULMA();
    float PitchAl();
    float RollAl();
    float YawAl();
private:
    I2C_HandleTypeDef *hi2c1;
    uint8_t data;
    int16_t gyroEksen[3], accEksen[3];
    float gyroHesap[3];
    int16_t accToplamVektor;
    float accPitchAci, accRollAci;
    float gyroRollAci, gyroPitchAci, gyroYawAci;
    float rollAci, pitchAcisi;
    int16_t hamSicaklik;
    float Sicaklik[1];

};


#endif /* MYOTONOMDRIVERS_INC_IMU_H_ */
