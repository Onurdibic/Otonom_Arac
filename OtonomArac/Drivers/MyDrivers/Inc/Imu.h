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

class MyImu {
public:
    MyImu(I2C_HandleTypeDef *hi2c);
    void gyroOffset(float *gyroCal);
    void readData(int16_t *accRaw, int16_t *gyroRaw, int16_t &rawTemp, float &temp);
    void memwrite(int8_t *data);
    void veriIyilestir(int16_t *gyroRaw);
    void veriOku();
private:
    I2C_HandleTypeDef *hi2c_;
    static const uint8_t MPU6050_ADDR ;
    static const uint8_t PWR_MGMT_1_REG ;
    static const uint8_t SMPLRT_DIV_REG ;
    static const uint8_t GYRO_CNFG_REG ;
    static const uint8_t ACC_CNFG_REG ;
};


#endif /* MYOTONOMDRIVERS_INC_IMU_H_ */
