/*
 * Imu.cpp
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */
#include "Imu.h"


#define PWR_MGMT_1_REG 0X6B
#define SMPLRT_DIV_REG 0X19
#define GYRO_CNFG_REG 0X1B
#define ACC_CNFG_REG 0X1C
#define MPU6500_ADDRESS 0x68<< 1
#define WHO_AM_I 0x75
#define RESET_BIT 0x80

MyImu::MyImu(I2C_HandleTypeDef *hi2c1)
{
	this->hi2c1=hi2c1;
}

void MyImu::DBC_MPU6500_Reset()
{
	data = RESET_BIT;
	HAL_I2C_Mem_Write(hi2c1, MPU6500_ADDRESS , PWR_MGMT_1_REG, 1, &data, 1, 1000);
	HAL_Delay(100);
}
void MyImu::DBC_MPU6500_YAPILANDIR()
{
	uint8_t check;
	DBC_MPU6500_Reset();
	// Check device ID WHO_AM_I
	HAL_I2C_Mem_Read(hi2c1, MPU6500_ADDRESS , WHO_AM_I, 1, &check, 1, 1000);
	if (check == 0x70)
	{
		// Power management register 0X6B we should write all 0's to wake the sensor up
		data = 0;
		HAL_I2C_Mem_Write(hi2c1, MPU6500_ADDRESS , PWR_MGMT_1_REG, 1, &data, 1, 1000);

		// Set Data Rate of 1KHz by writing SMPLRT_DIV register
		data = 0x07;
		HAL_I2C_Mem_Write(hi2c1, MPU6500_ADDRESS ,SMPLRT_DIV_REG, 1, &data, 1, 1000);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		data = 0x00;
		HAL_I2C_Mem_Write(hi2c1, MPU6500_ADDRESS, ACC_CNFG_REG, 1, &data, 1, 1000);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		data = 0x00;
		HAL_I2C_Mem_Write(hi2c1, MPU6500_ADDRESS , GYRO_CNFG_REG, 1, &data, 1, 1000);
	}
}

void MyImu::DBC_GYRO_OFSET()
{
	for(int i=0; i<2000; i++)
	{
		DBC_GYRO_OKU();

		gyroHesap[0] += gyroEksen[0];
	    gyroHesap[1] += gyroEksen[1];
	    gyroHesap[2] += gyroEksen[2];
	 }
		gyroHesap[0] /= 2000;
		gyroHesap[1] /= 2000;
	    gyroHesap[2] /= 2000;
	    //HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	    HAL_Delay(100);
}
void MyImu::DBC_READ_DATA()
{
    DBC_ACC_OKU();

    DBC_SICAKLIK_OKU();

    DBC_GYRO_OKU();

    gyroEksen[0] -= gyroHesap[0];
    gyroEksen[1] -= gyroHesap[1];
    gyroEksen[2] -= gyroHesap[2];
}
void MyImu::DBC_ACC_OKU()
{
	uint8_t tuffer[6];
	// Acc. Raw Values
	tuffer[0] = 0x3B;
	HAL_I2C_Master_Transmit(hi2c1, MPU6500_ADDRESS, tuffer, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(hi2c1, MPU6500_ADDRESS, tuffer, 6, HAL_MAX_DELAY);
	accEksen[0] = (tuffer[0] << 8 | tuffer[1]);
	accEksen[1] = (tuffer[2] << 8 | tuffer[3]);
	accEksen[2] = (tuffer[4] << 8 | tuffer[5]);
}
void MyImu::DBC_SICAKLIK_OKU()
{
	uint8_t buffer[2];
	// Temperature Values
	buffer[0] = 0x41;
	HAL_I2C_Master_Transmit(hi2c1, MPU6500_ADDRESS, buffer, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(hi2c1, MPU6500_ADDRESS, buffer, 2, HAL_MAX_DELAY);
	hamSicaklik = (buffer[0] << 8 | buffer[1]);
	Sicaklik[0]=((float)((float)hamSicaklik / 340.0)) + 36.53;
}
void MyImu::DBC_GYRO_OKU()
{
	uint8_t cuffer[6];
	 // Gyro Raw Values
	cuffer[0] = 0x43;
	HAL_I2C_Master_Transmit(hi2c1, MPU6500_ADDRESS, cuffer, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(hi2c1, MPU6500_ADDRESS, cuffer, 6, HAL_MAX_DELAY);
	gyroEksen[0] = (cuffer[0] << 8 | cuffer[1]);
	gyroEksen[1] = (cuffer[2] << 8 | cuffer[3]);
	gyroEksen[2] = (cuffer[4] << 8 | cuffer[5]);

}
void MyImu::DBC_ACI_BULMA()
{
	DBC_READ_DATA();//0.00140.000001066
	//HAM VERI iyilestirmeleri
	gyroPitchAci += gyroEksen[0] * 0.00140; //65.5 * 4000 mikrosaniye
	gyroRollAci += gyroEksen[1] * 0.00140;
	gyroYawAci += gyroEksen[2] * 0.00140;

	gyroPitchAci += gyroRollAci * sin(gyroEksen[2]* 0.000001066);
	gyroRollAci -= gyroPitchAci * sin(gyroEksen[2]* 0.000001066);

	accToplamVektor = sqrt((accEksen[0]*accEksen[0])+(accEksen[1]*accEksen[1])+(accEksen[2]*accEksen[2]));

	//57.296 =1 /(3.142 /180)
	accPitchAci = asin((float)accEksen[1]/accToplamVektor)*57.296;
	accRollAci = asin((float)accEksen[0]/accToplamVektor)*-57.296;

	pitchAcisi = gyroPitchAci * 0.9 + accPitchAci * 0.1;
	rollAci = gyroRollAci * 0.9 + accRollAci * 0.1;

}

float MyImu::PitchAl()
{
    return pitchAcisi;
}

float MyImu::RollAl()
{
    return rollAci;
}
float MyImu::YawAl()
{
    return gyroYawAci;
}
