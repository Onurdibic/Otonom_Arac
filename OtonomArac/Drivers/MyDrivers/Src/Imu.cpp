/*
 * Imu.cpp
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */
#include "Imu.h"

extern UART_HandleTypeDef huart2;

MyImu::MyImu(I2C_HandleTypeDef *hi2c) : hi2c_(hi2c) {}
void MyImu::memwrite(int8_t *data)
{
	// PWR_MGMT_1_REG
	//data=0x00;
	//HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,PWR_MGMT_1_REG,1,&data,1,HAL_MAX_DELAY);
	//GYRO CNFG -->+-500 derece/saniye --> 08
	//data=0x08;
	//HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,GYRO_CNFG_REG,1,&data,1,HAL_MAX_DELAY);
    //ACC CNFG --> +-8g -->10
    //data=0x10;
    //HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR,ACC_CNFG_REG,1,&data,1,HAL_MAX_DELAY);
}
void MyImu::gyroOffset(float *gyroCal) {
    uint8_t buffer[6];
    int16_t gyroRaw[3];

    for (int i = 0; i < 2000; ++i) {
        //buffer[0] = 0x43; // Start register for gyro is 43

        //HAL_I2C_Master_Transmit(hi2c_, MPU6050_ADDR, buffer, 1, HAL_MAX_DELAY);
        //HAL_I2C_Master_Receive(hi2c_, MPU6050_ADDR, buffer, 6, HAL_MAX_DELAY);

        gyroRaw[0] = (buffer[0] << 8 | buffer[1]);
        gyroRaw[1] = (buffer[2] << 8 | buffer[3]);
        gyroRaw[2] = (buffer[4] << 8 | buffer[5]);

        gyroCal[0] += gyroRaw[0];
        gyroCal[1] += gyroRaw[1];
        gyroCal[2] += gyroRaw[2];

        HAL_Delay(3);
    }

    gyroCal[0] /= 2000;
    gyroCal[1] /= 2000;
    gyroCal[2] /= 2000;
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
    HAL_Delay(1000);
}

void MyImu::readData(int16_t *accRaw, int16_t *gyroRaw, int16_t &rawTemp, float &temp) {
    uint8_t accBuffer[6], tempBuffer[2], gyroBuffer[6];

    // Read accelerometer raw values
    //accBuffer[0] = 0x3B;
    //HAL_I2C_Master_Transmit(hi2c_, MPU6050_ADDR, accBuffer, 1, HAL_MAX_DELAY);
    //HAL_I2C_Master_Receive(hi2c_, MPU6050_ADDR, accBuffer, 6, HAL_MAX_DELAY);
    accRaw[0] = (accBuffer[0] << 8 | accBuffer[1]);
    accRaw[1] = (accBuffer[2] << 8 | accBuffer[3]);
    accRaw[2] = (accBuffer[4] << 8 | accBuffer[5]);

    // Read temperature values
    //tempBuffer[0] = 0x41;
    //HAL_I2C_Master_Transmit(hi2c_, MPU6050_ADDR, tempBuffer, 1, HAL_MAX_DELAY);
    //HAL_I2C_Master_Receive(hi2c_, MPU6050_ADDR, tempBuffer, 2, HAL_MAX_DELAY);
    rawTemp = (tempBuffer[0] << 8 | tempBuffer[1]);
    temp = ((float)rawTemp / 340.0f) + 36.53f;

    // Read gyroscope raw values
    //gyroBuffer[0] = 0x43;
    //HAL_I2C_Master_Transmit(hi2c_, MPU6050_ADDR, gyroBuffer, 1, HAL_MAX_DELAY);
    //HAL_I2C_Master_Receive(hi2c_, MPU6050_ADDR, gyroBuffer, 6, HAL_MAX_DELAY);
    gyroRaw[0] = (gyroBuffer[0] << 8 | gyroBuffer[1]);
    gyroRaw[1] = (gyroBuffer[2] << 8 | gyroBuffer[3]);
    gyroRaw[2] = (gyroBuffer[4] << 8 | gyroBuffer[5]);
}
void MyImu::veriOku() {

	uint8_t txData[] = "IMU data\r\n";
//  HAL_UART_Transmit(&huart2, txData, sizeof(txData)-1, HAL_MAX_DELAY);

}
/*
void MyImu::veriIyilestir(int16_t *gyroRaw)
{
	 //HAM VERI iyilestirmeleri
	angle_pitch_gyro += gyro_raw[0] * 0.0000611; //65.5 * 4000 mikrosaniye
    angle_roll_gyro += gyro_raw[1] * 0.0000611;

	angle_pitch_gyro += angle_roll_gyro * sin(gyro_raw[2]* 0.000001066);
	angle_roll_gyro -= angle_pitch_gyro * sin(gyro_raw[2]* 0.000001066);

	acc_total_vector = sqrt((acc_raw[0]*acc_raw[0])+(acc_raw[1]*acc_raw[1])+(acc_raw[2]*acc_raw[2]));

	//57.296 =1 /(3.142 /180)
	angle_pitch_acc = asin((float)acc_raw[1]/acc_total_vector)*57.296;
	angle_roll_acc = asin((float)acc_raw[0]/acc_total_vector)*-57.296;

    if(set_gyro)
	{
	angle_pitch = angle_pitch_gyro * 0.9996 + angle_pitch_acc * 0.0004;
	angle_roll = angle_roll_gyro * 0.9996 + angle_roll_acc * 0.0004;
	}
	else
	{
	angle_pitch = angle_pitch_acc;
	set_gyro = true ;
	}

}*/


