#pragma once


#ifndef __TINBES03__ARDUINOS__FILESYSTEM__
#define __TINBES03__ARDUINOS__FILESYSTEM__


#include <Arduino.h>


float convertBytesToFloat(byte *arr);
void  convertFloatToBytes(float num, byte *arr);


#endif
