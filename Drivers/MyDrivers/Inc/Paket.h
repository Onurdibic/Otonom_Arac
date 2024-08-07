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

class Paket
{
public:
	Paket(UART_HandleTypeDef* huart);
    Paket(uint8_t baslik1_u8, uint8_t baslik2_u8, uint8_t paketTipi_u8, uint8_t dataBoyutu_u8);

    void GpsPaketOlustur(float latitude,float longitude,float altitude,float derece);
    void ImuPaketOlustur(float pitch,float roll,float yaw,float sicaklik);
    void VersiyonPaketGonder(uint8_t b,uint8_t o,uint8_t s);
    void YoklamaPaketGonder();
    void gpsPaketCagir(uint8_t *kopyaDizi);
    void imuPaketCagir(uint8_t *kopyaDizi);

    void PaketKesmeInit();
    void BayrakKaldir();
    void PaketCoz();

    float *ArayuzLatAl();
    float *ArayuzLonAl();

    uint8_t CRC8Hesaplama(uint8_t *data, uint8_t start, uint8_t end);
    bool PaketCozBayrak=false;
    bool YoklamaFlag=false;
    bool GidilecekNoktaBayrak=false;

private:
    UART_HandleTypeDef* huart;

    uint32_t floatToBytes(float *koordinatDeger_f, uint8_t* bytes_u8);
    float bytesToFloat(const uint8_t* buffer_u8, int32_t startIndex_s32);
    uint8_t gpspaket[21];
    uint8_t imupaket[21];
    uint8_t versiyonpaket[8];
    uint8_t yoklamapaket[8];

    uint8_t ArayuzData;
    uint8_t ArayuzBuffer_u8[120];
    uint8_t ArayuzIndex_u8=0;

    uint8_t pitchBytes_u8[4];
    uint8_t rollBytes_u8[4];
    uint8_t headingBytes_u8[4];
    uint8_t sicaklikBytes_u8[4];

    uint8_t latBytes_u8[4];
    uint8_t lonBytes_u8[4];
    uint8_t altBytes_u8[4];
    uint8_t dereceBytes_u8[4];

    uint8_t baslik1_u8;
    uint8_t baslik2_u8;
    uint8_t paketTipi_u8;
    uint8_t dataBoyutu_u8;
    int16_t dataLength_s16 = 0;
    uint32_t startIndex_u32 = 0;
    uint32_t intBits_u32 = 0;

    float latitude=0;
    float longitude=0;
    float altitude=0;
    float derece=0;
    float pitch=0;
	float roll=0;
	float heading=0;
	float sicaklik=0;
	float floatsonuc_f=0 ;

	float ArayuzEnlem_f = 0;
	float ArayuzBoylam_f = 0;
};

#endif /* MYDRIVERS_INC_PAKET_H_ */
