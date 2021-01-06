/*
 * 2C6.c
 *
 * Created: 3/7/2020 6:38:49 PM
 * Author : Owner
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
	DDRB |= (1<<5);		// PB5 set as output
	PORTB |= (1<<5);
	DDRC &= ~(1<<1);	// PC1 set as input
	PORTC &= (1<<1);	// pull-up
	
	OCR0A = 249; //top set
	TCCR0A |= (1<<WGM01); // CTC mode
	TCCR0B |= (1<<CS02)|(1<<CS00);		// prescaler of 1024
	TIMSK0 = (1<<OCIE0A); //timer compare enabled

	sei(); //interrupt enabled
	
	while (1)
	{
		
	}
}

ISR(TIMER0_COMPA_vect)
{
	TCNT0 = 0; //reset TCNT

		while (!(PINC & (1<<PINC1)))
		{
		PORTB ^= (1<<5);
		
		for (int i = 0; i <= 122; i++)
		{
			while (!(TIFR0 & (1<<OCF0A)))
			{
				//delay of 2 seconds.
			}
			TCNT0 = 0;
			TIFR0 |= (1<<OCF0A);
		}
		PORTB ^= (1<<5);
		TCNT0 = 0;
	}

}
