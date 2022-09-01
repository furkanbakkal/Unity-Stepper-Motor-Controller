#include <18F4520.h>
#device ADC=10

#fuses NOWDT      //No Watch Dog Timer

#use delay(crystal=20MHz)
#use timer(timer=0,tick=100us,bits=32,NOISR)
#define TICK_TYPE unsigned int32

