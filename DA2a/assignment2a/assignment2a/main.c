/*
 * assignment2a.c
 *
 * Created: 2/25/2020 2:19:12 PM
 * Author : Owner
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << 5); // the port b becomes an output at pin 2.
	PORTB |= (1<<5); // the port b has an output at pin 2.
	DDRC &= (0 << 1);
	PORTC |= (1 << 1);
	while (1) {
		if (!(PINC & (1<<PINC1)))
		{
			PORTB &= ~(1<<5);
			
			_delay_ms(2000);
		}
		else
		PORTB |= (1<<5);
		
	};
	
	return 0;
}


