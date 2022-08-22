#include <18F4520.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer

#use delay(internal=8MHz)
#use timer(timer=0,tick=100us,bits=32,NOISR)
#define TICK_TYPE unsigned int32
