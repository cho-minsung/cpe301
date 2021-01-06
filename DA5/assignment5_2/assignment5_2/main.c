/*
 * assignment5_2.c
 *
 * Created: 4/26/2020 8:18:38 PM
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
#define SHIFT_REGISTER DDRB
#define SHIFT_PORT PORTB
#define DATA (1<<PB3)
#define LATCH (1<<PB2)
#define CLOCK (1<<PB5)

//code from Dr. Venki's Github
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

//functions from previous assignments(3B)
void adc_init();
void read_adc();

//the function to allocate the temperatures into the 7-segment display
void convert(int num);

//code from Dr. Venki's Github made to be functions
void spi_init();
void spi_shift(uint8_t data1, uint8_t data2);

volatile float temp = 0;

int main(void)
{
	spi_init();
	adc_init();
	
	while (1){
		read_adc();
		display((int)temp); // display the temperature on the 7-segment displays
	}
}

void spi_init()
{
	SHIFT_REGISTER |= (DATA | LATCH | CLOCK);
	SHIFT_PORT &= ~(DATA | LATCH | CLOCK);
	SPCR0 = (1<<SPE) | (1<<MSTR);
	SHIFT_PORT &= ~LATCH;
}

void spi_shift(uint8_t data1, uint8_t data2)
{
	SPDR0 = data1;
	while(!(SPSR0 & (1<<SPIF)));
	
	SPDR0 = data2;
	while(!(SPSR0 & (1<<SPIF)));
	
	SHIFT_PORT |= LATCH;
	SHIFT_PORT &= ~LATCH;
}

void convert(int num)
{
	int ones = (num % 10);		//the first digit
	int tens = (num / 10) % 10;	//the second digit

	for (int i = 0; i < 2; i++)
	{
		if (i == 0 && num >= 10)
			spi_shift(SEGMENT_MAP[tens],SEGMENT_SELECT[2]);
		else
			spi_shift(SEGMENT_MAP[ones],SEGMENT_SELECT[3]);
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