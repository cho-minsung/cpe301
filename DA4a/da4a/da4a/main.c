/*
 * da4a.c
 *
 * Created: 4/4/2020 1:46:33 PM
 * Author : Owner
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

void adc_init(void);
void read_adc(void);

int main()
{
	float poten;
	
	//the ADC setup
	ADMUX = (1<<REFS0) | (1<<ADLAR);
		/*
		Reference selection bits,
		AVcc external cap at AREF,
		left-adjusted,
		ADC5 channel selected*/
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);
		/*ADC enabled,
		auto trigger enabled,
		interrupt flag initialized and disabled,
		prescaler: 101*/
	
	//the pin setup
	DDRC  |=  ~(1<<3);			//PC3 is an input
	PORTC |=  (1<<3) | (1<<0);	//Activate Pull- up resistor PC0 & 3
	DDRD  |=  (1<<DDD6);		//PD6 is an output for OCR0A
	
	//the interrupt setup
	PCICR = (1<<PCIE1);		//Pin change interrupt control register
	PCMSK1 = (1<<PCINT11);	//Mask register is enabled for for the input
	sei ();					//the interrupt is enabled.
	
	//the pulse Width Modulation setup
	TCCR0A |= (1<<COM0A1)|(0<<COM0A0)|(1<<WGM00)|(0<<WGM01);	//Clear OC0A, CPC PWM, phase correct with top as OCR0A
	TCCR0B |= (0<<WGM02)|(1<<CS00);								//Pre-scaling is 1
	OCR0A = 0;													//the top value is initialized
	
	while (1)
	{
		//reading the ADC value 
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		poten = ADC;
		
		_delay_ms(10);	//had an error if not included
		
		//16 bit max = 0xFFFF = 65535
		//95% is 65535 * 0.95 = 62258.25 = 62258(less than 95%)
		if (poten >= 62258)
		{
			/*the DC cycle calculation
			OCR0A = 95% * 0xFF(256) - 1 = 242*/
			OCR0A = 242; //top = 242, which is a max
			_delay_ms(10); //somewhat bearable latency
		}
		else if ((poten <= 62257) && (poten > 0))
		{
			//the speed control
			OCR0A = 241* (poten / 62257);
			_delay_ms(10);
		}
		else
		OCR0A = 0;
	}
}

ISR(PCINT1_vect)
{
	PORTC ^= (1<<0);	//switch toggle
}
