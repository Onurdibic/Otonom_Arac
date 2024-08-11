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
float Isaret::ustSinirla(float isaret_f,uint32_t sinirdeger_u32 )
{
	if(isaret_f > sinirdeger_u32 )
	{
		isaret_f = sinirdeger_u32;
	}
	return isaret_f;
}
float Isaret::altSinirla(float isaret_f,uint32_t sinirdeger_u32 )
{
	if(isaret_f < sinirdeger_u32 )
	{
		isaret_f = sinirdeger_u32;
	}
	return isaret_f;
}

