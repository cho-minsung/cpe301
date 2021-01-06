/*
 * 2C5.c
 *
 * Created: 3/7/2020 6:26:51 PM
 * Author : Owner
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
	DDRB |= (1<<3);		// PB3 set as output
	
	TCCR0A |= (1<<WGM01); // CTC mode
	TCCR0B |= (1<<CS02) | (1<<CS00); // set prescaler. 1024
	TIMSK0 = (1<<OCIE0A); //timer0 Compare enabled
	
	OCR0A = 249; //top is set for 0.75s of period
	sei(); //interrupt enabled

	while (1)
	{
		
	}
}

ISR(TIMER0_COMPA_vect)
{
	TCNT0 = 0;
	TCNT0 = 0;
	// 55% duty cycle, 412.5s
	for (int i = 0; i <= 24; i++)
	{
		while (!(TIFR0 & (1<<OCF0A)))
		{
		}
		TCNT0 = 0; // reset TCNT
		TIFR0 |= (1<<OCF0A); //clear the flag
	}
	PORTB &= ~(1<<3);

	TCNT0 = 0;
	
	//45% duty cycle, 337.5s
	for (int j = 0; j < 21; j++)
	{
		while (!(TIFR0 & (1<<OCF0A)))
		{
		}
		TCNT0 = 0;
		TIFR0 |= (1<<OCF0A);
	}
	PORTB |= (1<<3);
	TCNT0 = 0;
}
