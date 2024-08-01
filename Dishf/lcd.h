/*
 * LCD - H File
 *
 */ 


#ifndef LCD_H_
#define LCD_H_

#ifndef F_CPU
#define F_CPU 3000000UL 		//Your clock speed in Hz (3Mhz here)
#endif
#include <avr/io.h>
#include <util/delay.h>

#define LCD_DPRT PORTB
#define LCD_DDDR DDRB 
#define LCD_RS	 0
#define LCD_EN   1

void LCD_cmd(unsigned char cmnd);
void LCD_write(unsigned char data);
void LCD_init_LCD(void);
void LCD_Write_String(char *str);
void LCD_Set();

#endif /* LCD_H_ */