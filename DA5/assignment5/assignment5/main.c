/*
 * assignment5.c
 *
 * Created: 4/26/2020 5:13:07 PM
 * Author : Owner
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdio.h>
#include <stdlib.h>

//code from Dr. Venki's Github
#define LATCH 4
#define CLOCK 7
#define DATA 0
#define LSBFIRST 0
#define MSBFIRST 1

//code from Dr. Venki's Github
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

//functions from previous assignments(3B)
void adc_init();
void read_adc();

//functions from Dr. Venki's Github
void shift_out_init(void);
void shift_out(uint8_t indata);

//the function to allocate the temperatures into the 7-segment dislplay
void convert(int num);

volatile float temp = 0;

int main(void)
{
	//the setups
	adc_init();
	shift_out_init();
	
	while (1)
	{
		read_adc();
		convert((int)temp);	//changes the temperature into integer
	}
}

void adc_init()
{
	ADMUX |= (1 << REFS0) | (0 << MUX3) | (1 << MUX2) | (0 << MUX1) | (0 << MUX0);
	ADCSRA |= (1 << ADEN) | (0 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	DDRC = 0;
	PORTC = 0;
}

void read_adc()
{
	temp = 0;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	temp = ADC;
	temp = (temp / 1024) * 500;
}

void shift_out_init(void)
{
	DDRB|=(1 << DATA);
	DDRD|=(1 << CLOCK) | (1 << LATCH);
}

void shift_out(uint8_t indata)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (0 == (indata & _BV(7 - i)))
			PORTB &= (0 << DATA);
		else
			PORTB |= (1 << DATA);

		PORTD |= (1 << CLOCK);
		PORTD &= (0 << CLOCK);
	}
}

void convert(int num)
{
	int ones = (num % 10);		//the first digit
	int tens = (num / 10) % 10;	//the second digit

	for (int i = 0; i < 2; i++)
	{
		if (i == 0 && num >= 10)
		{
			PORTD &= (0 << LATCH);
			shift_out(SEGMENT_MAP[tens]);
			shift_out(SEGMENT_SELECT[2]);
			PORTD |= (1 << LATCH);
		}
		else
		{
			PORTD &= (0 << LATCH);
			shift_out(SEGMENT_MAP[ones]);
			shift_out(SEGMENT_SELECT[3]);
			PORTD |= (1 << LATCH);
		}
	}
}