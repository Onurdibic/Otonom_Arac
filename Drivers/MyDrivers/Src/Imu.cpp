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

MyImu::MyImu(I2C_HandleTypeDef *hi2c)
{
	this->hi2c=hi2c;
}

void MyImu::DBC_MPU6500_Reset()
{
	data_u8 = RESET_BIT;
	HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS , PWR_MGMT_1_REG, 1, &data_u8, 1, 100);
	HAL_Delay(100);
}
void MyImu::DBC_MPU6500_YAPILANDIR()
{
	uint8_t check_u8;
	DBC_MPU6500_Reset();
	// yoklama sorgusu versiyon sorgusu crc kontrol ,

	HAL_I2C_Mem_Read(hi2c, MPU6500_ADDRESS , WHO_AM_I, 1, &check_u8, 1, 1000);
	if (check_u8 == 0x70)
	{
		data_u8 = 0x00;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS , PWR_MGMT_1_REG, 1, &data_u8, 1, 1000);
		// SMPLRT_DIV register
		data_u8 = 0x07;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS ,SMPLRT_DIV_REG, 1, &data_u8, 1, 1000);
		//  ACCEL_CONFIG Register ±2g(00),±4g(01),±8g(10),±16g(11) 4:3
		data_u8 = 0x00;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS, ACC_CNFG_REG, 1, &data_u8, 1, 1000);
		//  GYRO_CONFIG Register +250dps(00),+500dps(01),+1000dps(10),+2000dps(11) 4:3
		data_u8 = 0x00;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS , GYRO_CNFG_REG, 1, &data_u8, 1, 1000);
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
void MyImu::DBC_DATA_OKU()
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
	uint8_t accBuffer[6];

	accBuffer[0] = 0x3B;
	HAL_I2C_Master_Transmit(hi2c, MPU6500_ADDRESS, accBuffer, 1, 10);
	HAL_I2C_Master_Receive(hi2c, MPU6500_ADDRESS, accBuffer, 6, 10);
	accEksen[0] = (accBuffer[0] << 8 | accBuffer[1]);
	accEksen[1] = (accBuffer[2] << 8 | accBuffer[3]);
	accEksen[2] = (accBuffer[4] << 8 | accBuffer[5]);
}
void MyImu::DBC_SICAKLIK_OKU()
{
	uint8_t sicaklikBuffer[2];

	sicaklikBuffer[0] = 0x41;
	HAL_I2C_Master_Transmit(hi2c, MPU6500_ADDRESS, sicaklikBuffer, 1, 10);
	HAL_I2C_Master_Receive(hi2c, MPU6500_ADDRESS, sicaklikBuffer, 2, 10);
	hamSicaklik_u16 = (sicaklikBuffer[0] << 8 | sicaklikBuffer[1]);
	Sicaklik_f=((float)((float)hamSicaklik_u16 / 340.0)) + 36.53;
}
void MyImu::DBC_GYRO_OKU()
{
	uint8_t gyroBuffer[6];

	gyroBuffer[0] = 0x43;
	HAL_I2C_Master_Transmit(hi2c, MPU6500_ADDRESS, gyroBuffer, 1, 10);
	HAL_I2C_Master_Receive(hi2c, MPU6500_ADDRESS, gyroBuffer, 6, 10);
	gyroEksen[0] = (gyroBuffer[0] << 8 | gyroBuffer[1]);
	gyroEksen[1] = (gyroBuffer[2] << 8 | gyroBuffer[3]);
	gyroEksen[2] = (gyroBuffer[4] << 8 | gyroBuffer[5]);

}
void MyImu::DBC_ACI_BULMA()
{
	DBC_DATA_OKU();//0.0014 0.000001066
	//Ham Veri Iyilestirmeleri
	gyroPitchAci_f += gyroEksen[0] * 0.000207;
	gyroRollAci_f += gyroEksen[1] * 0.000207;
	gyroYawAci_f += gyroEksen[2] * 0.000207;

	gyroPitchAci_f += gyroRollAci_f * sin(gyroEksen[2]* 0.000001066);
	gyroRollAci_f -= gyroPitchAci_f * sin(gyroEksen[2]* 0.000001066);

	accToplamVektor_s16 = sqrt((accEksen[0]*accEksen[0])+(accEksen[1]*accEksen[1])+(accEksen[2]*accEksen[2]));

	//57.296 =1 /(3.142 /180)
	accPitchAci_f = asin((float)accEksen[1]/accToplamVektor_s16)*57.296;
	accRollAci_f = asin((float)accEksen[0]/accToplamVektor_s16)*-57.296;

	pitchAcisi_f = gyroPitchAci_f * 0.9 + accPitchAci_f * 0.1;
	rollAci_f = gyroRollAci_f * 0.9 + accRollAci_f * 0.1;
}

float* MyImu::PitchAl()
{
    return &pitchAcisi_f;
}

float* MyImu::RollAl()
{
    return &rollAci_f;
}
float* MyImu::YawAl()
{
    return &gyroYawAci_f;
}
float* MyImu::SicaklikAl()
{
    return &Sicaklik_f;
}
