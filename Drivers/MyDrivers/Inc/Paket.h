/*
 * Paket.h
 *
 *  Created on: Jul 8, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_PAKET_H_
#define MYDRIVERS_INC_PAKET_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "string.h"

class Paket{
public:
	Paket(UART_HandleTypeDef* huart);
    Paket(uint8_t baslik1_u8, uint8_t baslik2_u8, uint8_t paketTipi_u8, uint8_t dataBoyutu_u8);

    void PaketOlustur(float latitude,float longitude);
    void PaketOlustur(float pitch,float roll,float yaw,float sicaklik);
    void PaketOlusturveGonder(uint8_t b,uint8_t o,uint8_t s);
    void gpsPaketCagir(uint8_t *kopyaDizi);
    void imuPaketCagir(uint8_t *kopyaDizi);

    void PaketKesmeInit();
    void BayrakKaldir();
    void PaketCoz();
    uint8_t CRC8Hesaplama(uint8_t *data, uint8_t start, uint8_t end);
    bool PaketCozBayrak=false;
private:
    UART_HandleTypeDef* huart;

    uint32_t floatToBytes(float *koordinatDeger_f, uint8_t* bytes);
    float bytesToFloat(const uint8_t* buffer, int32_t startIndex);
    uint8_t gpspaket[14];
    uint8_t imupaket[21];
    uint8_t ArayuzData[12];
    uint8_t pitchBytes_u8[4];
    uint8_t rollBytes_u8[4];
    uint8_t headingBytes_u8[4];
    uint8_t sicaklikBytes_u8[4];

    uint8_t latBytes_u8[4];
    uint8_t lonBytes_u8[4];
    uint8_t rxData_u8;
    uint8_t rxIndex_u8;
    uint8_t baslik1_u8;
    uint8_t baslik2_u8;
    uint8_t paketTipi_u8;
    uint8_t dataBoyutu_u8;
    int16_t dataLength_s16 = 0;
    uint32_t startIndex_u32 = 0;
    uint32_t intBits_u32 = 0;
    float latitude;
    float longitude;
    float pitch;
	float roll;
	float heading;
	float sicaklik;
	float sonuc ;
	float ArayuzEnlem_f = 0;
	float ArayuzBoylam_f = 0;
};

#endif /* MYDRIVERS_INC_PAKET_H_ */
