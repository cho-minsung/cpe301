#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
	 PORTC |=  (1<<3); //pull up
	 DDRD |= 0xF0; //PORTD as Output
	 
	 //CCW: AIN1 = L, AIN2 = H, PWM = H
	 //CW: AIN1 = H, AIN2 = L, PWM = H
	 
	 while(1)
	 {
		 PORTD = 0x10; //counter clock wise
		 _delay_ms(5000);
		 PORTD = 0x00; //no movement
		 _delay_ms(5000);
		 PORTD = 0x20; //clock wise
		 _delay_ms(5000);
		 PORTD = 0x30; //no movement
		 _delay_ms(5000);
	}
}