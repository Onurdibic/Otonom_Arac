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
    void PaketCagir(uint8_t *kopyaDizi);

private:
    uint32_t bytesToFloat(float *koordinatDeger_f, uint8_t* bytes);
    uint8_t paket[12];
    uint8_t baslik1_u8;
    uint8_t baslik2_u8;
    uint8_t paketTipi_u8;
    uint8_t dataBoyutu_u8;
    float latitude;
    float longitude;
};

#endif /* MYDRIVERS_INC_PAKET_H_ */
