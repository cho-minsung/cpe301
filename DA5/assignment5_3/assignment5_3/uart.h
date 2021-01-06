#ifndef USART_RS232_H_FILE_H_				/* Define library H file if not defined */
#define USART_RS232_H_FILE_H_

#define F_CPU 16000000UL						/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)	/* Define prescale value */

void USART_Init();				/* USART initialize function */
void USART_SendString(char*);				/* Send string of USART data function */


#endif	