/*
 * hx711.c
 *
 * Created: 2020-07-14 오후 4:20:09
 *  Author: ParkJaeHwan
 */ 

#include "hx711.h"
#include <avr/io.h>

#define sbi(PORTX, BitX) PORTX |=  (1<<BitX)
#define cbi(PORTX, BitX) PORTX &= ~(1<<BitX)

void UART1_TX(unsigned char data)
{
	while((UCSR1A & 0x20)== 0x00);
	UDR1 = data;
}

void UART1_TX_int(unsigned long data)
{
	unsigned long temp = 0;
	
	temp = data/10000;
	UART1_TX(temp+48);
	temp = (data&10000)/1000;
	UART1_TX(temp+48);
	temp = (data&1000)/100;
	UART1_TX(temp+48);
	temp = (data&100)/10;
	UART1_TX(temp+48);
	temp = (data&10);
	UART1_TX(temp+48);
}

unsigned long  ReadCout(void);

volatile unsigned long weight = 0;
volatile unsigned long offset = 0;
volatile int offset_flag = 0;




unsigned long ReadCout(void)
{
	unsigned long sum=0, count=0, data1=0, data2=0;
	
	for(int j=0; j<32; j++)
	{
		sbi(PORTA,0);	// dout 1
		cbi(PORTA,1);	// sck 0
		
		count = 0;
		
		while((PINA & 0b00000001) == 0b00000001);
		
		for(int i=0; i<24; i++)
		{
			sbi(PORTA, 1); // sck 1
			count = count << 1;
			cbi(PORTA, 1); // sck 0
			if((PINA & 0b00000001) == 0b00000001){
				count++;
			}
		}
		sbi(PORTA, 1);	// sck 1
		count = count^0x800000;
		cbi(PORTA, 1);	// sck 0
		
		sum += count;
	}
	data1 = sum/32;	// 32개 데이터 합한 것을 평균
	
	if(offset_flag == 0)
	{
		offset = data1;
		offset_flag = 1;
	}
	
	if(data1 > offset){
		data2 = data1 - offset;
	} else data2 = 0;
	
	return data2;
}


void init()
{
	DDRA = 0b00000010; // A0(DOUT) : input A1(SCK):output
	
	UCSR1A = 0b00000000;
	UCSR1B = 0b00001000;
	UCSR1C = 0b00000110;
	UBRR1H = 0;
	UBRR1L = 103;
	
	while(1)
	{
		weight = ReadCout()/4;	// 무게는 weight = ReadCout()/4
		UART1_TX_int(weight);
		UART1_TX('\n');
		UART1_TX('\r');
	}
}
