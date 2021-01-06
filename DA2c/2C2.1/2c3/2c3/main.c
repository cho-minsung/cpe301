/*
 * 2c3.c
 *
 * Created: 3/7/2020 6:09:58 PM
 * Author : Owner
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
	DDRB |= (1<<3);		// PB3 set as output
	TCCR0A = 0x00;		// normal mode
	TCCR0B = 0X05;		// set prescaler. 1024
	TIMSK0 = (1<<TOIE0); //timer0 interrupt enabled
	sei(); //interrupt enabled

	while (1)
	{
		
	}
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 0;
	TCNT0 = 0;
	// 55% duty cycle, 412.5s
	for (int i = 0; i <= 24; i++)
	{
		while (TCNT0 != 255)
		{
		}
		TCNT0 = 0; // reset TCNT
	}
	PORTB &= ~(1<<3);
	
	TCNT0 = 0;
	
	//45% duty cycle, 337.5s
	for (int j = 0; j < 21; j++)
	{
		while (TCNT0 != 255)
		{
		}
		TCNT0 = 0;
	}
	PORTB |= (1<<3);
	TCNT0 = 0;
}