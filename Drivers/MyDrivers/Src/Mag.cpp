#include "Mag.h"

#define HMC5883L_ADDRESS 0x1E << 1 // I2C adresi

HMC5883L::HMC5883L(I2C_HandleTypeDef *hi2c)
{
  this->hi2c = hi2c;
  x = y = z = 0;
  heading = headingAcisi = 0.0f;
}

void HMC5883L::Yapilandir()
{
  // Configuration Register A
	registerYaz(HMC5883_REGISTER_MAG_CRA_REG_M, 0x70); // 8 ortalama, 15 Hz, normal ölçüm

  // Configuration Register B
	registerYaz(HMC5883_REGISTER_MAG_CRB_REG_M, HMC5883_MAGGAIN_1_3); // Kazanç = +/- 1.3

  // Mode Register
	registerYaz(HMC5883_REGISTER_MAG_MR_REG_M, 0x00); // Sürekli ölçüm modu
}

void HMC5883L::registerYaz(uint8_t reg, uint8_t value)
{
  uint8_t data[2] = {reg, value};
  HAL_I2C_Master_Transmit(hi2c, HMC5883L_ADDRESS, data, 2, HAL_MAX_DELAY);
}

void HMC5883L::registerOku(uint8_t reg, uint8_t *value)
{
  HAL_I2C_Master_Transmit(hi2c, HMC5883L_ADDRESS, &reg, 1, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(hi2c, HMC5883L_ADDRESS, value, 1, HAL_MAX_DELAY);
}

void HMC5883L::MagDataOku(int16_t *x, int16_t *y, int16_t *z)
{
  uint8_t buffer[6];
  HAL_I2C_Mem_Read(hi2c, HMC5883L_ADDRESS, HMC5883_REGISTER_MAG_OUT_X_H_M, I2C_MEMADD_SIZE_8BIT, buffer, 6, HAL_MAX_DELAY);

  *x = (int16_t)((buffer[0] << 8) | buffer[1]);
  *z = (int16_t)((buffer[2] << 8) | buffer[3]);
  *y = (int16_t)((buffer[4] << 8) | buffer[5]);
}
void HMC5883L::KalibreEt()
{
    int16_t xRaw, yRaw, zRaw;
    int16_t xMin = 3200, yMin = 3200;
    int16_t xMax = -3200, yMax = -3200;

    for (int i = 0; i < 1000; i++)
    {
        MagDataOku(&xRaw, &yRaw, &zRaw);

        if (xRaw < xMin) xMin = xRaw;
        if (xRaw > xMax) xMax = xRaw;
        if (yRaw < yMin) yMin = yRaw;
        if (yRaw > yMax) yMax = yRaw;

        HAL_Delay(10);
    }

    xOffset = (xMax + xMin) / 2;
    yOffset = (yMax + yMin) / 2;
}
float HMC5883L::HeadingOlustur()
{
	float xCalibrated, yCalibrated;
	MagDataOku(&x,&y,&z);

	xCalibrated = x - xOffset;
	yCalibrated = y - yOffset;

	heading = atan2(yCalibrated, xCalibrated);

	heading = atan2((y), (x));
	if(heading < 0)
		heading += 2*M_PI;

	if(heading > 2*M_PI)
		heading -= 2*M_PI;


	headingAcisi = heading * (180/M_PI);

	return headingAcisi;

}
