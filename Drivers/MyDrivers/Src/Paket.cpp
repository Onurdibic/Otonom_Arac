#include "Paket.h"
#include <string.h>

enum Durumlar
{
	Baslik1Coz,
	Baslik2Coz,
	PaketTuruSec,
	DataBoyutuAl,
	DataOku

};
enum Paketler
{
	GPS=0x01 ,
	VERSIYON=0x02

};

Paket::Paket(UART_HandleTypeDef* huart)
{
	this->huart=huart;

}
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
	this->heading=0;
	this->sicaklik=0;
    memset(gpspaket, 0, sizeof(gpspaket));
    memset(imupaket, 0, sizeof(imupaket));
}
void Paket::PaketKesmeInit()
{
	HAL_UART_Receive_IT(huart, ArayuzData, sizeof(ArayuzData));
}
void Paket::PaketOlustur(float latitude,float longitude)
{

    gpspaket[0] = 0x12;
    gpspaket[1] = 0x34;
    gpspaket[2] = 0x01;
    gpspaket[3] = 0x09;
    this->latitude = latitude;
    this->longitude = longitude;

    floatToBytes(&latitude, latBytes_u8);
    floatToBytes(&longitude, lonBytes_u8);

    memcpy(gpspaket + 4, latBytes_u8, 4);
    memcpy(gpspaket + 8, lonBytes_u8, 4);
    gpspaket[13]=CRC8Hesaplama(gpspaket,4, 12);
}
void Paket::PaketOlustur(float pitch,float roll,float heading,float sicaklik)
{
    imupaket[0] = 0x12;
    imupaket[1] = 0x34;
    imupaket[2] = 0x02;
    imupaket[3] = 0x11;
    this->pitch = pitch;
    this->roll = roll;
    this->heading = heading;
    this-> sicaklik = sicaklik;

    floatToBytes(&pitch, pitchBytes_u8);
    floatToBytes(&roll, rollBytes_u8);
    floatToBytes(&heading, headingBytes_u8);
    floatToBytes(&sicaklik, sicaklikBytes_u8);

    memcpy(imupaket + 4, pitchBytes_u8, 4);
    memcpy(imupaket + 8, rollBytes_u8, 4);
    memcpy(imupaket + 12, headingBytes_u8, 4);
    memcpy(imupaket + 16, sicaklikBytes_u8, 4);

    imupaket[20] = CRC8Hesaplama(imupaket, 4,20);
}
void Paket::PaketOlusturveGonder(uint8_t b,uint8_t o,uint8_t s)
{
	ArayuzData[2]=0x03;
	ArayuzData[4]=b;
	ArayuzData[5]=o;
	ArayuzData[6]=s;
	HAL_UART_Transmit(huart, ArayuzData, sizeof(ArayuzData), 1000);

}
void Paket::gpsPaketCagir(uint8_t *kopyaDizi)
{
	memcpy(kopyaDizi, gpspaket, sizeof(gpspaket));
}
void Paket::imuPaketCagir(uint8_t *kopyaDizi)
{
	memcpy(kopyaDizi, imupaket, sizeof(imupaket));
}

void Paket::BayrakKaldir()
{
	PaketCozBayrak=true;
}
void Paket::PaketCoz()
{
    Durumlar Durum = Baslik1Coz;
    Paketler Paket = VERSIYON;
    bool islem=true;
    while (islem)
    {
        switch (Durum)
        {
            case Baslik1Coz:
                if (ArayuzData[startIndex_u32] == 0x12 && ArayuzData[startIndex_u32] != 0)
                {
                    Durum = Baslik2Coz;
                }
                startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzData);
                break;

            case Baslik2Coz:
                if (ArayuzData[startIndex_u32] == 0x34 && ArayuzData[startIndex_u32] != 0)
                {
                    Durum = PaketTuruSec;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzData);
                }
                else
                {
                    Durum = Baslik1Coz;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzData);
                }
                break;

            case PaketTuruSec:
                if (ArayuzData[startIndex_u32] != 0)
                {
                    Paket = (Paketler)ArayuzData[startIndex_u32];
                    Durum = DataBoyutuAl;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzData);
                }
                else
                {
                    Durum = Baslik1Coz;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzData);
                }
                break;

            case DataBoyutuAl:
                if (ArayuzData[startIndex_u32] != 0)
                {
                    dataLength_s16 = ArayuzData[startIndex_u32];
                    Durum = DataOku;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzData);
                }
                else
                {
                    Durum = Baslik1Coz;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzData);
                }
                break;

            case DataOku:
                if (Paket == GPS && dataLength_s16 == 8)
                {
                    ArayuzEnlem_f = bytesToFloat(ArayuzData, startIndex_u32);
                    ArayuzBoylam_f = bytesToFloat(ArayuzData, (startIndex_u32 + 4) % sizeof(ArayuzData));


                    if (ArayuzEnlem_f != 0 && ArayuzBoylam_f != 0)
                    {

                    }

                    startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzData);
                    Durum = Baslik1Coz;
                }
                if (Paket == VERSIYON && dataLength_s16 == 8)
                {

                	PaketOlusturveGonder(0,0,4);
                	startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzData);
                	Durum = Baslik1Coz;
                }
                else
                {
                	Durum = Baslik1Coz;
                	islem=false;
                }
                break;
        }
    }

    HAL_UART_Receive_IT(huart, ArayuzData, sizeof(ArayuzData));
}

uint8_t Paket::CRC8Hesaplama(uint8_t *data, uint8_t start ,uint8_t end)
{
    uint8_t crc = 0x00;

    for (uint8_t i = start; i < end; i++)
    {
        crc ^= data[i]; // CRC değerini, dizinin bir sonraki byte ile XOR
        for (uint8_t j = 0; j < 8; j++) //Her bir byte için döngü
        {
            if (crc & 0x80)//CRC değerinin en soldaki biti 1 mi
            {
                crc = (crc << 1) ^ 0X07; //En yüksek bit birse CRC değerini bir bit sola kaydır ve XOR işlemi yap
            }
            else
            {
                crc <<= 1; //En yüksek bit sıfırsa CRC değerini bir bit sola kaydır
            }
        }
    }

    return crc;
}

float Paket::bytesToFloat(const uint8_t* buffer, int32_t startIndex) {
   intBits_u32 =(buffer[(startIndex + 3) % 120] << 24) |
    		(buffer[(startIndex + 2) % 120] << 16) |
			(buffer[(startIndex + 1) % 120] << 8)  |
			(buffer[(startIndex + 0) % 120] << 0)  ;

    memcpy(&sonuc, &intBits_u32, sizeof(sonuc));
    return sonuc;
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
