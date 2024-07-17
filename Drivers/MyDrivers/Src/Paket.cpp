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
	this->pitch=0;
	this->roll=0;
	this->yaw=0;
	this->sicaklik=0;
    memset(gpspaket, 0, sizeof(gpspaket));
    memset(imupaket, 0, sizeof(imupaket));
}

void Paket::PaketOlustur(float latitude,float longitude)
{
	uint8_t latBytes_u8[4];
	uint8_t lonBytes_u8[4];
    gpspaket[0] = baslik1_u8;
    gpspaket[1] = baslik2_u8;
    gpspaket[2] = paketTipi_u8;
    gpspaket[3] = dataBoyutu_u8;
    this->latitude = latitude;
    this->longitude = longitude;

    floatToBytes(&latitude, latBytes_u8);
    floatToBytes(&longitude, lonBytes_u8);

    for(int i = 0; i < 4; i++)
    {
        gpspaket[7 - i] = latBytes_u8[i];
        gpspaket[11 - i] = lonBytes_u8[i];
    }
}
void Paket::PaketOlustur(float pitch,float roll,float yaw,float sicaklik)
{
    uint8_t pitchBytes_u8[4];
    uint8_t rollBytes_u8[4];
    uint8_t yawBytes_u8[4];
    uint8_t sicaklikBytes_u8[4];
    imupaket[0] = baslik1_u8;
    imupaket[1] = baslik2_u8;
    imupaket[2] = paketTipi_u8;
    imupaket[3] = dataBoyutu_u8;
    this->pitch = pitch;
    this->roll = roll;
    this->yaw = yaw;
    this-> sicaklik = sicaklik;

    floatToBytes(&pitch, pitchBytes_u8);
    floatToBytes(&roll, rollBytes_u8);
    floatToBytes(&yaw, yawBytes_u8);
    floatToBytes(&sicaklik, sicaklikBytes_u8);

    for(int i = 0; i < 4; i++)
    {
        imupaket[7 - i] = pitchBytes_u8[i];
        imupaket[11 - i] = rollBytes_u8[i];
        imupaket[15 - i] = yawBytes_u8[i];
        imupaket[19 - i] = sicaklikBytes_u8[i];
    }
}
void Paket::gpsPaketCagir(uint8_t *kopyaDizi)
{
	memcpy(kopyaDizi, gpspaket, sizeof(gpspaket));
}
void Paket::imuPaketCagir(uint8_t *kopyaDizi)
{
	memcpy(kopyaDizi, imupaket, sizeof(imupaket));
}
uint32_t Paket::floatToBytes(float *Deger_f, uint8_t* bytes)
{
    uint8_t* p = (uint8_t*)Deger_f;
    for (int i = 0; i < 4; i++)
    {
        bytes[i] = p[i];
    }
    return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0] & 0xFF);
}
