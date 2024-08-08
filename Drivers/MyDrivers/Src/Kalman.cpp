/*
 * Kalman.cpp
 *
 *  Created on: Aug 8, 2024
 *      Author: onurd
 */


#include "Kalman.h"

KalmanFiltresi::KalmanFiltresi(float islemGurultu, float olcumGurultu, float tahminHata)
{
	Q=islemGurultu;
	R=olcumGurultu;
	P=tahminHata;
	K=0;
	X=0;
}
float KalmanFiltresi::guncelle(float olcum)
{
    K = P / (P + R);
    X = X + K * (olcum - X);
    P = (1 - K) * P + Q;

    return X;
}

