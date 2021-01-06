/*
 * midterm1.c
 *
 * Created: 3/30/2020 7:11:20 PM
 * Author : Minsung Cho
 */ 

#define F_CPU 16000000
#define BAUD 9600

#include <util/setbaud.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include <stdio.h>

uint8_t ascii_2_int(uint8_t n)
{
	if(n >= 48 && n <= 57) //outside of integers
	{
		send_byte();
		return n - 48;
	}
	return 1;
}

void send_byte(uint8_t byte)
{
	if(UCSR0B & (1<<TXEN0))
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = byte;
	}
}

void send_str(char string[])
{
	int i = 0;
	while (string[i] != '\0')
		send_byte(string[i++]);
}

char get_byte()
{
	while(!(UCSR0A & (1<<7)));
	return UDR0;
}


uint16_t analog_conv()
{
	ADCSRA |= (1<<6);				//ADSC is enabled for conversion.
	while (!(ADCSRA&(1<<4)));		
	ADCSRA |= (1<<4);				//ADIF flag is cleared.
	return ADC;
}

float dig_2_temp(uint16_t temp, char mode)
{
	float temp_converted = temp*110/1023;
	switch (mode) {
		case 't':
		return temp_converted;			//Temperature in Celsius is returned.
		case 'T':
		default:
		return temp_converted*1.8 + 32;	//Temperature in Fahrenheit is returned.
	}
}

int main(void)
{
	//ADC is enabled here
	DDRC &= ~(1<<4);		//pin4 is set as input.
	ADMUX = 0xC4;			//1100 0100, internal 1.1V ref, right justified, ADC4.
	ADCSRA = 0x00;			//0000 0000, free running.
	ADCSRB = 0x87;			//1000 0111, enables ADC, prescaler of 128.
	
	//UART is enabled here
	DDRD |= 0x02;			//port is set.
	UCSR0C = 0x06;			//Asynchronous, no parity.
	
	//transmitter is enabled here
	UCSR0B |= ~(1<<RXEN0);
	
	//receiver is enabled here
	DDRD &= ~1;
	UCSR0B |= (1<<RXEN0);
	
	//LED is enabled here
	DDRB |= (1<<2);
	PORTB &= ~(1<<2);
	DDRB |= (1<<5);
	PORTB &= ~(1<<5);
	
	//timer0 and timer1 are initialized here
	OCR1A = 15624;			//An approximation of 1 second for 16Mhz 1024 prescaler.
	TIMSK1 = 0x02;			//compare match A interrupt is enabled.
	TIFR1 |= 0x02;			//interrupt flag is cleared.
	sei();
	TCCR1B = 0x0D;			//timer1 is set as CTC mode with 1024 prescaler.
	TCCR1A = 0x00;
	TCNT1 = 0;
	
	//various arrays are set
	const char menu[] = "h: Help screen \n" 
						"t: Displays temperature in C \n"
						"T: Displays temperature in F \n"
						"o: Turns ON LED on PB5 \n"
						"O: Turns OFF LED on PB5 \n"
						"s: Sends a string to the terminal \n"
						"i: Sends an integer to the terminal \n \n";
	volatile char temp_string[5];
	volatile char integer[1];
	
	//menu will pop up
	send_str(menu);
	
	while(1)
	{
		char instruction = get_byte();
		if(instruction == 'h')
			send_str(menu);
		else if(instruction == 't' || 'T')
		{
			uint16_t temperature = analog_conv();
			float temp_converted = dig_2_temp(temperature, instruction);
			snprintf(temp_string, 5, "%f\n", temp_converted);
			send_str(temp_string);
			send_byte('\n');
		}
		else if( instruction == 'o')
			PORTB &= ~(1<<5);
		else if(instruction == 'O')
			PORTB |= (1<<5);
		else if(instruction == 's')
			send_str("I love Dr. Venki");
		else if(instruction == 'i')
		{
			snprintf(integer, 1, "%d\n", 1);
			send_str(integer);
			send_byte('\n');
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	TIFR1 |= 0x02;
	int delay_time = 0;
	delay_time = (delay_time + 1) %1;
	if (delay_time == 0)
		PORTB ^= (1<<2);
}