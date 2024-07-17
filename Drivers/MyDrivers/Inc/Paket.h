/*
 * Paket.h
 *
 *  Created on: Jul 8, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_PAKET_H_
#define MYDRIVERS_INC_PAKET_H_


#include <stdint.h>
#include "string.h"
class Paket{
public:
    Paket(uint8_t baslik1_u8, uint8_t baslik2_u8, uint8_t paketTipi_u8, uint8_t dataBoyutu_u8);
    void PaketOlustur(float latitude,float longitude);
    void PaketOlustur(float pitch,float roll,float yaw,float sicaklik);
    void gpsPaketCagir(uint8_t *kopyaDizi);
    void imuPaketCagir(uint8_t *kopyaDizi);
private:
    uint32_t floatToBytes(float *koordinatDeger_f, uint8_t* bytes);
    uint8_t gpspaket[12];
    uint8_t imupaket[20];
    uint8_t baslik1_u8;
    uint8_t baslik2_u8;
    uint8_t paketTipi_u8;
    uint8_t dataBoyutu_u8;
    float latitude;
    float longitude;
    float pitch;
	float roll;
	float yaw;
	float sicaklik;
};

#endif /* MYDRIVERS_INC_PAKET_H_ */
