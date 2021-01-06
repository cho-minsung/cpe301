/*
 * 3a2.c
 *
 * Created: 3/11/2020 9:28:48 PM
 * Author : Owner
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <stdio.h>

void USART_init(void);
void USART_tx_string(char *data);

int main(void)
{
	USART_init();
	
	int num, i;
	char snum1[8];
	char snum2[10];
	
	TCCR0A = 0x00;
	TCCR0B = 0x05;
	TCNT0 = 0;
	
	
	while (1)
	{
		num = rand();
		itoa(num, snum1, 5);
		float dec = ((rand() * 2.2));
		snprintf(snum2, sizeof(snum2), "%f", dec);
		USART_tx_string("This is how much I love Embedded Systems: ");
		USART_tx_string(snum1);
		USART_tx_string(" ");
		USART_tx_string(snum2);
		USART_tx_string("\n");
		
		for (int i = 0; i <= 61; i++)
		{
			while (TCNT0 != 255)
			{
				
			}
			TCNT0 = 0;
		}
	}
}

/* INIT USART (RS-232) */
void USART_init( void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B = (1<<TXEN0);	// enable TX & RX interrupt
	UCSR0C = (3<<UCSZ00);	// asynchronous 8 N 1
}

/* SEND A STRING TO THE RS-232 */
void USART_tx_string(char *data)
{
	while((*data != '\0')) // while there's still data
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data; // data is written into UDR0
		data++;
	}
}
