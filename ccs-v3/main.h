#include <18F46K22.h>
#device ADC=10

#fuses NOWDT      //No Watch Dog Timer

#use delay(internal=64MHz)
#device HIGH_INTS = TRUE 
#use timer(timer=0,tick=100us,bits=32,NOISR)
#define TICK_TYPE unsigned int32

