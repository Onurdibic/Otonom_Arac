/*
 * Kalman.cpp
 *
 *  Created on: Aug 8, 2024
 *      Author: onurd
 */


#include "Kalman.h"

KalmanFiltresi::KalmanFiltresi(float islemGurultu_f, float olcumGurultu_f, float tahminHata_f)
{
	Q=islemGurultu_f;
	R=olcumGurultu_f;
	P=tahminHata_f;
	K=0;
	X=0;
}
float KalmanFiltresi::veriGuncelle(float veri_f)
{
    K = P / (P + R);
    X = X + K * (veri_f - X);
    P = (1 - K) * P + Q;

    return X;
}

