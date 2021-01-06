/*
 * 2c4.c
 *
 * Created: 3/7/2020 6:17:32 PM
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
	
	TCCR0A = 0x00;		// normal mode
	TCCR0B = 0X05;		// prescaler of 1024
	
	TIMSK0 = (1<<TOIE0); //timer 0 overflow interrupt
	sei(); //interrupt enabled
	
	while (1)
	{
		
	}
}

ISR(TIMER0_OVF_vect)
{
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