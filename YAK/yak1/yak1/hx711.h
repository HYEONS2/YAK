/*
 * hx711.h
 *
 * Created: 2020-07-14 오후 4:20:22
 *  Author: ParkJaeHwan
 */ 


#ifndef HX711_H_
#define HX711_H_
#include <avr/io.h>

void weight_init();
unsigned long  ReadCout(void);
void init();


#endif /* HX711_H_ */