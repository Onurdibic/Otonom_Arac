/*
 * IsaretIsleme.cpp
 *
 *  Created on: Aug 10, 2024
 *      Author: onurd
 */
#include "IsaretIsleme.h"

Isaret::Isaret()
{

}
float Isaret::ustSinirla(float isaret_f,float sinirdeger_f )
{
	if(isaret_f > sinirdeger_f )
	{
		isaret_f = sinirdeger_f;
	}
	return isaret_f;
}
float Isaret::altSinirla(float isaret_f,float sinirdeger_f )
{
	if(isaret_f < sinirdeger_f )
	{
		isaret_f = sinirdeger_f;
	}
	return isaret_f;
}

