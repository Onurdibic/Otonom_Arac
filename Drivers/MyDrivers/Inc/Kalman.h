/*
 * Kalman.h
 *
 *  Created on: Aug 8, 2024
 *      Author: onurd
 */

#ifndef MYDRIVERS_INC_KALMAN_H_
#define MYDRIVERS_INC_KALMAN_H_

class KalmanFiltresi
{
public:
    KalmanFiltresi(float islemGurultu, float olcumGurultu, float tahminHata);

    float guncelle(float olcum);

private:
    float Q; // İşlem gürültüsü kovaryansı
    float R; // Ölçüm gürültüsü kovaryansı
    float P; // Tahmin hatası kovaryansı
    float K; // Kalman kazancı
    float X; // Tahmin edilen değer
};



#endif /* MYDRIVERS_INC_KALMAN_H_ */
