/*
 * 4b2.c
 *
 * Created: 4/11/2020 7:59:02 PM
 * Author : Owner
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

void read_adc(void);

float poten;

int main(void)
{
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	ICR1 = 4999;
	//fPWM = focs/N(TOP+1) = 50Hz, 20ms
	DDRB |= (1<<PB1);// PWM pin set as output
	
	ADMUX = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
	ADCSRA=(1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);
	//the same setup as the first set.
	
	while(1)
	{
		read_poten();
		if (poten >= 1023)
		{
			OCR1A = 535; //180 degree
			_delay_ms(100);
		}
		else if (poten <= 20)
		{
			OCR1A = 97;	//0 degree
			_delay_ms(100);
		}
	}
}

void read_poten(void)
{
	unsigned char i=4;
	poten = 0;
	while(i--) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		poten += ADC;
	}
	poten = poten/4;
}
