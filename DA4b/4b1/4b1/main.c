/*
 * 4b1.c
 *
 * Created: 4/11/2020 3:15:29 PM
 * Author : Owner
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>

volatile float poten;

void read_poten(void);

int main(void)
{
	DDRD = 0x0F;
	DDRC |= ~(1<<0);
	TCCR1B |= (1 << WGM12)|(1 << CS11)|(1 << CS10);
	/*PD0-PD3 are outputs
	PC0 is an input
	CTC, prescaler = 64
	*/
	
		ADMUX = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
		/*Reference selection bits
		AVcc - external cap at AREF
		right adjust result
		Analog channel selection bits
		ADC5*/
		
		ADCSRA=(1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);
		// ADCSRA setup
	
	while (1)
	{
		read_poten();
		
		//Timer Count = (10ms/(1/(16000000/64)))- 1 = 2499
		//Timer Count = (20ms/(1/(16000000/64)))- 1 = 4999
		//Timer Count = (30ms/(1/(16000000/64)))- 1 = 7499
		//Timer Count = (40ms/(1/(16000000/64)))- 1 = 9999

		
		if (poten >= 768)
		OCR1A = 9999;
		else if (poten >= 512)
		OCR1A = 2499;
		else if (poten >= 256)
		OCR1A = 4999;
		else if (poten >= 0)
		OCR1A = 7499;
		
		PORTD = 0x0A;					// Clockwise spin
		TIFR1 |= (1 << OCF1A);			//the flag bit is cleared
		while(!(TIFR1 & (1 << OCF1A)));	//loop till the flag is set
		PORTB = 0x06;
		TIFR1 |= (1 << OCF1A);
		while(!(TIFR1 & (1 << OCF1A)));
		PORTD = 0x05;
		TIFR1 |= (1 << OCF1A);
		while(!(TIFR1 & (1 << OCF1A)));
		PORTD = 0x09;
		TIFR1 |= (1 << OCF1A);
		while(!(TIFR1 & (1 << OCF1A)));
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