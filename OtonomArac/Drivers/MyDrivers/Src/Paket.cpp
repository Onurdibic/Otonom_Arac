/*
 * Paket.cpp
 *
 *  Created on: Jul 8, 2024
 *      Author: onurd
 */
#include "Paket.h"
#include <string.h>

Paket::Paket(uint8_t baslik1_u8, uint8_t baslik2_u8, uint8_t paketTipi_u8, uint8_t dataBoyutu_u8)
{
	this->baslik1_u8=baslik1_u8;
	this->baslik2_u8=baslik2_u8;
	this->paketTipi_u8=paketTipi_u8;
	this->dataBoyutu_u8=dataBoyutu_u8;
	this->latitude=0;
	this->longitude=0;
    memset(paket, 0, sizeof(paket));
}

void Paket::PaketOlustur(float latitude,float longitude)
{
    paket[0] = baslik1_u8;
    paket[1] = baslik2_u8;
    paket[2] = paketTipi_u8;
    paket[3] = dataBoyutu_u8;
    this->latitude = latitude;
    this->longitude = longitude;
    uint8_t latBytes_u8[4];
    uint8_t lonBytes_u8[4];

    bytesToFloat(&latitude, latBytes_u8);
    bytesToFloat(&longitude, lonBytes_u8);

    for(int i = 0; i < 4; i++)
    {
        paket[7 - i] = latBytes_u8[i];
        paket[11 - i] = lonBytes_u8[i];
    }
}
void Paket::PaketCagir(uint8_t *kopyaDizi)
{
	memcpy(kopyaDizi, paket, sizeof(paket));
}
uint32_t Paket::bytesToFloat(float *koordinatDeger_f, uint8_t* bytes)
{
    uint8_t* p = (uint8_t*)koordinatDeger_f;
    for (int i = 0; i < 4; i++)
    {
        bytes[i] = p[i];
    }
    return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0] & 0xFF);
}
