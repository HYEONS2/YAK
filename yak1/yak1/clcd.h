/*
 * clcd.h
 *
 * Created: 2020-07-21 오후 2:16:07
 *  Author: ParkJaeHwan
 */ 


#ifndef CLCD_H_
#define CLCD_H_

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include "i2c.h"
#include <util/delay.h>

#include <avr/io.h>
#include <stdint.h>

void twi_init(void);
void i2c_lcd_init(void);
void i2c_lcd_command(uint8_t command);
void i2c_lcd_command_8(uint8_t command);
void i2c_lcd_data(uint8_t data);
void i2c_lcd_goto_XY(uint8_t row, uint8_t col);
void i2c_lcd_string(uint8_t row, uint8_t col, char string[]);

#endif /* CLCD_H_ */