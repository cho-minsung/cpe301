/*
 * 2c2.c
 *
 * Created: 3/7/2020 5:20:05 PM
 * Author : Owner
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>

int main()
{
	DDRB |= (1<<5);		// PB5 set as output
	PORTB |= (1<<5);
	DDRC &= ~(1<<1);	// PC1 set as input
	PORTC &= (1<<1);	// pull-up
	
	TCCR0A = 0x00;		// normal mode
	TCCR0B = 0X05;		// prescaler of 1024
	TCNT0 = 0; //reset TCNT
			
	while (1) {
		while (!(PINC & (1<<PINC1)))
		{		
			PORTB ^= (1<<5);
			
			for (int i = 0; i <= 122; i++)
			{
				while (TCNT0 != 255)
				{
					//delay of 2 seconds.
				}
				TCNT0 = 0;
			}
			PORTB ^= (1<<5);
			TCNT0 = 0;
		}
	}
}

