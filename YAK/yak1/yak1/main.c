#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hx711.h"

volatile int check0 = 0;	// 포토 인터럽트0
volatile int check1 = 0;	// 포토 인터럽트1
volatile char receive = 0;	// 어플 받아오는 값
volatile char send = 0;		// 어플 주는 값

volatile int check_yak = 0;		// 약이 통안에 있는지 파악하기 위한 변수
volatile int check_time = 0;	// 약 시간인지 아닌지 파악하기 위한 변수

volatile int motor_sel = 0; // 1:A 2:B 3:C .. 어떤 모터를 돌릴지 선택할 변수 switch case ..

void motorA()
{
	OCR1A = 3100; // +90 deg
	_delay_ms(1000);
	OCR1A = 1000; // -90 deg
	_delay_ms(1000);
}

void motorB()
{
	OCR1B = 3100;	// +90 deg
	_delay_ms(1000);
	OCR1B = 1000;	// -90 deg
	_delay_ms(1000);
}

void motorC()	// 잠금장치 ON OFF
{
	OCR1C = 3100;	// +90 deg
	_delay_ms(1000);
// 	OCR1C = 1000;	// -90 deg
// 	_delay_ms(1000);
}

void motorC2()	// 잠금장치 ON OFF
{
	OCR1C = 1000;	// 돌아가기 전으로
}

ISR(INT0_vect) // PORTD0 포토 인터럽트0
{
	check0 = 1;
}

ISR(INT1_vect) // PORTD1 포토 인터럽트1
{
	check1 = 1;
}

void uart_init() {
	UCSR0A=0x00;
	// ...
	UCSR0B=0x18;
	// RXEN, TXEN
	UCSR0C=0x06;
	// 8Bit
	UBRR0H=0;
	// 9600
	UBRR0L=103;
	// 9600
	sei();
}

void uart_send(char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

char uart_receive(void)
{
	while(!(UCSR0A & 0x80));
	//수신완료 flag 비트가 1되면 정지
	return UDR0;
	//UART0번 사용
}

int main(void)
{
	DDRB=0b11100000;   // PB567 out
	TCCR1A=0b10101010; TCCR1B=0x1A;  ICR1=19999; // OCR1A -> OC Clear / Fast PWM TOP = ICR1 / 8분주
	//OCR1A=3000; OCR1B=3000;
	// TCCR1B=0x1A; OCR1A=3000; ICR1=19999;
	uart_init();

	// External Interrupt(s) initialization
	EICRA=0b00001010; //외부인터럽트 01번핀 트리거 신호를  falling edge 설정
	EICRB=0x00; 
	EIMSK=0x03; //INT0 1번핀을 외부인터럽트 핀으로 설정, 0 1번 외부인터럽트 활성화	
	
	sei();
	
	weight_init();	// 무게 측정 코드 레지스터 설정 함수
	
	/* 무게 알고리즘 미완성 ★★★★★★
	
	// 전원 ON 했을 때 무게 측정
	long init_weight[5]=0;
	for(int i=0; i<5; i++){
		init_weight[i]=((ReadCout())/4);
	}	// 5개의 배열안에 값을 넣음
	
	long temp_weight[5]=0;	
	
	★★★★★★★★★  */
	
	while(1)
	{
		// 약을 체크하는 코드 넣어야 함 (check_yak 변하게)
		// 무게는 weight = ReadCout()/4
		// ....
		
		if(check_yak == 1)
		{
			// 어플에서 시간이 맞으면 블루투스로 받아서 check_time 변하게 해야함 
			// ....
			
			receive = uart_receive();	// 폴링방식이여서 어플에서 안보내면 여기서 멈춤
			// 폴링 방식이 안된다면 인터럽트로 수신받아야 함
			
			if(receive == 'a'){
				// A약 시간이 맞으면 a 를 받음 (알람일때?)
				check_time = 1;
				motor_sel = 1;	// a모터 선택
			} 
			
			if(receive == 'b'){
				// B약 시간이 맞으면 b 를 받음 (알람일때?)
				check_time = 1;
				motor_sel = 2;	// b모터 선택
			}
			
			if(check_time == 1)
			{
				/*주요 로직*/
				switch(motor_sel)	// motor_sel=1 : A, 2:B
				{
					case 1 :
						while (check0 == 0)		// check0,1 변수는 포토인터럽트 모듈의 인터럽트에 의해서 변경됨
						{
							motorA();
						}	// 약이 떨어질 때 까지 motorA 돌리기
						motorC();	// 잠금장치 풀기
						break;
					
					case 2 :
						while (check1 == 0)		// check0,1 변수는 포토인터럽트 모듈의 인터럽트에 의해서 변경됨
						{
							motorB();
						}	// 약이 떨어질 때 까지 motorB 돌리기
						motorC();	// 잠금장치 풀기
						break;
					
					default:
						break;
				}
				check0=0; check1=0;	// 다시 약 체크상황 없는걸로 초기화
			} else { /*알람 보내기*/ }
		} else {  } // 이상태엔 할게 있나
	}
}
