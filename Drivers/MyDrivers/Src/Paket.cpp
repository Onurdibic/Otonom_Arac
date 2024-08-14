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
	ROTA=0x01 ,
	VERSIYON=0x02,
	YOKLAMA=0x03,
	DUR=0x04

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
void Paket::PaketKesmeYapilandir()
{
	HAL_UART_Receive_IT(huart, &ArayuzData,1);
}
void Paket::GpsPaketOlustur(float latitude,float longitude,float altitude,float derece)
{

    gpspaket[0] = baslik1_u8;
    gpspaket[1] = baslik2_u8;
    gpspaket[2] = paketTipi_u8;
    gpspaket[3] = dataBoyutu_u8;

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
    imupaket[0] = baslik1_u8;
    imupaket[1] = baslik2_u8;
    imupaket[2] = paketTipi_u8;
    imupaket[3] = dataBoyutu_u8;

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
void Paket::VersiyonPaketOlustur(uint8_t b,uint8_t o,uint8_t s)
{
	versiyonpaket[0] = baslik1_u8;
	versiyonpaket[1] = baslik2_u8;
	versiyonpaket[2] = paketTipi_u8;
	versiyonpaket[3] = dataBoyutu_u8;
	versiyonpaket[4] = b;
	versiyonpaket[5] = o;
	versiyonpaket[6] = s;
	versiyonpaket[7] = CRC8Hesaplama(versiyonpaket, 4,7);
}
void Paket::YoklamaPaketOlustur()
{
	yoklamapaket[0] = baslik1_u8;
	yoklamapaket[1] = baslik2_u8;
	yoklamapaket[2] = paketTipi_u8;
	yoklamapaket[3] = dataBoyutu_u8;
	yoklamapaket[4] = 0x01;
	yoklamapaket[5] = 0x02;
	yoklamapaket[6] = 0x03;
	yoklamapaket[7] = CRC8Hesaplama(yoklamapaket, 4,7);
}
void Paket::RotaPaketOlustur()
{
	rotapaket[0] = baslik1_u8;
	rotapaket[1] = baslik2_u8;
	rotapaket[2] = paketTipi_u8;
	rotapaket[3] = dataBoyutu_u8;
	rotapaket[4] = 0x11;
	rotapaket[5] = 0x22;
	rotapaket[6] = 0x33;
	rotapaket[7] = CRC8Hesaplama(rotapaket, 4,7);
}
void Paket::gpsPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, gpspaket, sizeof(gpspaket));}
void Paket::imuPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, imupaket, sizeof(imupaket));}
void Paket::versiyonPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, versiyonpaket, sizeof(versiyonpaket));}
void Paket::yoklamaPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, yoklamapaket, sizeof(yoklamapaket));}
void Paket::rotaPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, rotapaket, sizeof(rotapaket));}

void Paket::ArayuzDataAlveBayrakKaldir()
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
                if (Paket == ROTA && dataLength_s16 == 8)
                {
                	if (ArayuzBuffer_u8[(startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8)] == CRC8Hesaplama(ArayuzBuffer_u8,startIndex_u32,startIndex_u32 + dataLength_s16))
                	{
                		if(GidilecekNoktaBayrak==false)
                		{
                			ArayuzEnlem_f = bytesToFloat(ArayuzBuffer_u8, startIndex_u32);
                			ArayuzBoylam_f = bytesToFloat(ArayuzBuffer_u8, (startIndex_u32 + 4) % sizeof(ArayuzBuffer_u8));
                		}
                		GidilecekNoktaBayrak = true;
                		RotaGeldiBayrak=true;
                	}

                    startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8);
                    Durum = Baslik1Coz;
                    islem = false;
                }
                else if (Paket == VERSIYON && dataLength_s16 == 4)
                {
                	if (ArayuzBuffer_u8[(startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8)] == CRC8Hesaplama(ArayuzBuffer_u8,startIndex_u32,startIndex_u32 + dataLength_s16))
                	{
                		VersiyonPaketBayrak=true;
                	}

                    startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8);
                    Durum = Baslik1Coz;
                    islem = false;
                }
                else if (Paket == YOKLAMA && dataLength_s16 == 4)
                {
                	if (ArayuzBuffer_u8[(startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8)] == CRC8Hesaplama(ArayuzBuffer_u8,startIndex_u32,startIndex_u32 + dataLength_s16))
                	{
                		YoklamaFlag=true;
                		YoklamaPaketFlag=true;
                	}


                    startIndex_u32 = (startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8);
                    Durum = Baslik1Coz;
                    islem = false;
                }
                else if(Paket == DUR && dataLength_s16==4)
                {
                	if (ArayuzBuffer_u8[(startIndex_u32 + dataLength_s16) % sizeof(ArayuzBuffer_u8)] == CRC8Hesaplama(ArayuzBuffer_u8,startIndex_u32,startIndex_u32 + dataLength_s16))
                	{
                		arabaDurBayrak=true;
                	}

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

uint8_t Paket::CRC8Hesaplama(uint8_t *data, uint8_t baslangic ,uint8_t bitis)
{
    uint8_t crc = 0x00;

    for (uint8_t i = baslangic; i < bitis; i++)
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

float Paket::bytesToFloat(uint8_t* buffer_u8, int32_t startIndex_s32)
{
	uint32_t intBits_u32 =(buffer_u8[(startIndex_s32 + 3) % 120] << 24) |
    					(buffer_u8[(startIndex_s32 + 2) % 120] << 16) |
						(buffer_u8[(startIndex_s32 + 1) % 120] << 8)  |
						(buffer_u8[(startIndex_s32 + 0) % 120] << 0)  ;

    memcpy(&floatsonuc_f, &intBits_u32, sizeof(floatsonuc_f));
    return floatsonuc_f;
}

void Paket::floatToBytes(float *Deger_f, uint8_t* bytes)
{
    uint8_t* p = (uint8_t*)Deger_f;
    for (int i = 0; i < 4; i++)
    {
        bytes[i] = p[i];
    }
}
