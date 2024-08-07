#ifndef VERSIYON_H_
#define VERSIYON_H_

#define VERSIYON "0.0.7"


#define MAJOR 0
#define MINOR 0
#define BUILD 7

/******************************************************
*
* Versiyon    : 0.0.7
* Tarih       : 08.08.2024
* Yazanlar    : Onur Dibic
*
* STM32CubeIDE : 1.14.0
*
* Aciklamalar :
* - Kodumuz bu versiyonda araba ile birleştirildi.
* - Araba classına yeni metodlar eklenerek arabanın hareketleri yazıldı ve test edildi.
* - YonelmeGorev 2 Hz sıklıkta aracın en kısa şekilde nereden yönelebileceğini bulmustur.
* Sensor hesaplamalarının ve sistemin yön bulunana kadar durduğu gözlemlenmiştir. Çözüm olarak ise
* görev içerisinde bayrak kaldırıp loop içerisinde yönelimi kontrol etmek olarak bulunmuştur.
* - Kodun ve aracın geldiği nokta ,arayüz üzerinden bir nokta gönderildikten sonra o noktaya yönelip
* ardından o noktaya ilerlenilip ,durmaktır.
*
******************************************************/

#endif  VERSIYON_H_
