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
	VERSIYON=0x02,
	YOKLAMA=0x03

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
}
void Paket::PaketKesmeInit()
{
	HAL_UART_Receive_IT(huart, &ArayuzData,1);
}
void Paket::GpsPaketOlustur(float latitude,float longitude,float altitude,float derece)
{

    gpspaket[0] = 0x12;
    gpspaket[1] = 0x34;
    gpspaket[2] = 0x01;
    gpspaket[3] = 0x11;
    this->latitude = latitude;
    this->longitude = longitude;
    this->altitude = altitude;
    this->derece = derece;
    floatToBytes(&latitude, latBytes_u8);
    floatToBytes(&longitude, lonBytes_u8);
    floatToBytes(&altitude, altBytes_u8);
    floatToBytes(&derece, dereceBytes_u8);
    memcpy(gpspaket + 4, latBytes_u8, 4);
    memcpy(gpspaket + 8, lonBytes_u8, 4);
    memcpy(gpspaket + 12, altBytes_u8, 4);
    memcpy(gpspaket + 16, dereceBytes_u8, 4);
    gpspaket[20]=CRC8Hesaplama(gpspaket,4, 20);
}
void Paket::ImuPaketOlustur(float pitch,float roll,float heading,float sicaklik)
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
void Paket::VersiyonPaketGonder(uint8_t b,uint8_t o,uint8_t s)
{
	versiyonpaket[0] = 0x12;
	versiyonpaket[1] = 0x34;
	versiyonpaket[2] = 0x03;
	versiyonpaket[3] = 0x04;
	versiyonpaket[4] = b;
	versiyonpaket[5] = o;
	versiyonpaket[6] = s;
	versiyonpaket[7] = CRC8Hesaplama(versiyonpaket, 4,7);
	HAL_UART_Transmit(huart, versiyonpaket, sizeof(versiyonpaket), 1000);
}
void Paket::YoklamaPaketGonder()
{
	YoklamaFlag=true;
	yoklamapaket[0] = 0x12;
	yoklamapaket[1] = 0x34;
	yoklamapaket[2] = 0x04;
	yoklamapaket[3] = 0x04;
	yoklamapaket[4] = 0x01;
	yoklamapaket[5] = 0x02;
	yoklamapaket[6] = 0x03;
	yoklamapaket[7] = CRC8Hesaplama(yoklamapaket, 4,7);
	HAL_UART_Transmit(huart, yoklamapaket, sizeof(yoklamapaket), 1000);
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
	if (ArayuzData!=0xFF)
	{
		ArayuzBuffer_u8[ArayuzIndex_u8] = ArayuzData;
		ArayuzIndex_u8 = (ArayuzIndex_u8 + 1) % sizeof(ArayuzBuffer_u8);
	}
	else
	{
		PaketCozBayrak=true;
	}
	 HAL_UART_Receive_IT(huart, &ArayuzData, 1);
}
void Paket::PaketCoz()
{
    Durumlar Durum = Baslik1Coz;
    Paketler Paket = VERSIYON;
    bool islem = true;

    while (islem)
    {
        switch (Durum)
        {
            case Baslik1Coz:
            	if (ArayuzBuffer_u8[startIndex_u32] == 0x12 && ArayuzBuffer_u8[startIndex_u32] != 0)
                {
                    Durum = Baslik2Coz;
                }
                startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzBuffer_u8);
                break;

            case Baslik2Coz:
                if (ArayuzBuffer_u8[startIndex_u32] == 0x34 && ArayuzBuffer_u8[startIndex_u32] != 0)
                {
                    Durum = PaketTuruSec;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzBuffer_u8);
                }
                else
                {
                    Durum = Baslik1Coz;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzBuffer_u8);
                }
                break;

            case PaketTuruSec:
                if (ArayuzBuffer_u8[startIndex_u32] != 0)
                {
                    Paket = (Paketler)ArayuzBuffer_u8[startIndex_u32];
                    Durum = DataBoyutuAl;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzBuffer_u8);
                }
                else
                {
                    Durum = Baslik1Coz;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzBuffer_u8);
                }
                break;

            case DataBoyutuAl:
                if (ArayuzBuffer_u8[startIndex_u32] != 0)
                {
                    dataLength_s16 = ArayuzBuffer_u8[startIndex_u32];
                    Durum = DataOku;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzBuffer_u8);
                }
                else
                {
                    Durum = Baslik1Coz;
                    startIndex_u32 = (startIndex_u32 + 1) % sizeof(ArayuzBuffer_u8);
                }
                break;

            case DataOku:
                if (Paket == GPS && dataLength_s16 == 8)
                {
                    ArayuzEnlem_f = bytesToFloat(ArayuzBuffer_u8, startIndex_u32);
                    ArayuzBoylam_f = bytesToFloat(ArayuzBuffer_u8, (startIndex_u32 + 4) % sizeof(ArayuzBuffer_u8));
                    GidilecekNoktaBayrak = true;

                    startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8);
                    Durum = Baslik1Coz;
                    islem = false;
                }
                else if (Paket == VERSIYON && dataLength_s16 == 8)
                {
                    VersiyonPaketGonder(0, 0, 8);

                    startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8);
                    Durum = Baslik1Coz;
                    islem = false;
                }
                else if (Paket == YOKLAMA && dataLength_s16 == 8)
                {
                    YoklamaPaketGonder();
                    startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8);
                    Durum = Baslik1Coz;
                    islem = false;
                }
                else
                {
                    Durum = Baslik1Coz;
                    islem = false;
                }
                break;
        }
    }
}

float *Paket::ArayuzLatAl(){return &ArayuzEnlem_f;}
float *Paket::ArayuzLonAl(){return &ArayuzBoylam_f;}

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

float Paket::bytesToFloat(const uint8_t* buffer_u8, int32_t startIndex_s32)
{
	uint32_t intBits_u32 =(buffer_u8[(startIndex_s32 + 3) % 120] << 24) |
    					(buffer_u8[(startIndex_s32 + 2) % 120] << 16) |
						(buffer_u8[(startIndex_s32 + 1) % 120] << 8)  |
						(buffer_u8[(startIndex_s32 + 0) % 120] << 0)  ;

    memcpy(&floatsonuc_f, &intBits_u32, sizeof(floatsonuc_f));
    return floatsonuc_f;
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
