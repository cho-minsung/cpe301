/*
 * 2a2.c
 *
 * Created: 2/25/2020 8:26:41 PM
 * Author : Owner
 */ 

#include <avr/io.h>
#define F_CPU 1600000UL
#include <util/delay.h>

int main(void) {

	DDRB |= (1<<PB3);
	while(1) {
		PORTB |= (1<<PB3);
		_delay_ms(412.5);
		PORTB &= ~(1<<PB3);
		_delay_ms(337.5);
	}
	return 1;
}
