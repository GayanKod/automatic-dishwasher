
#include "lcd.h"

//This function is used to send required commands to the LCD
void LCD_cmd(unsigned char cmnd)
{
	LCD_DPRT = (LCD_DPRT & 0x0f)|(cmnd & 0xf0);		/* SEND COMMAND TO DATA PORT */
	LCD_DPRT &= ~ (1<<LCD_RS);						/* RS = 0 FOR COMMAND */
	LCD_DPRT |= (1<<LCD_EN);						/* EN = 1 FOR H TO L PULSE */
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE */
	LCD_DPRT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE */
	_delay_us(100);									/* WAIT FOR MAKE ENABLE WIDE */ //200
	
	LCD_DPRT = (LCD_DPRT & 0x0f)|(cmnd << 4);		/* SEND COMMAND TO DATA PORT */
	LCD_DPRT |= (1<<LCD_EN);						/* EN = 1 FOR H TO L PULSE */
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE */
	LCD_DPRT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE */
	_delay_us(2000);								/* WAIT FOR MAKE ENABLE WIDE */
}

//This function is used to display a single character on the LCD
void LCD_write(unsigned char data)
{
	LCD_DPRT = (LCD_DPRT & 0x0f)|(data & 0xf0);		/* SEND DATA TO DATA PORT  sending upper nibble*/
	LCD_DPRT |= (1<<LCD_RS);						/* MAKE RS = 1 FOR DATA */
	LCD_DPRT |= (1<<LCD_EN);						/* EN=1 FOR H TO L PULSE */
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE */
	LCD_DPRT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE */
	_delay_us(100);			//200						/* WAIT FOR MAKE ENABLE WIDE */
	
	LCD_DPRT = (LCD_DPRT & 0x0f)|(data << 4);		/* sending lower nibble */
	LCD_DPRT |= (1<<LCD_EN);						/* EN=1 FOR H TO L PULSE*/
	_delay_us(1);									/* WAIT FOR MAKE ENABLE WIDE*/
	LCD_DPRT &= ~(1<<LCD_EN);						/* EN = 0 FOR H TO L PULSE*/
	_delay_us(2000);								/* WAIT FOR MAKE ENABLE WIDE*/
}

void LCD_init_LCD(void)
{
	LCD_DDDR = 0xFF;								//Making DDRB pins as output pins
	_delay_ms(20);									/* WAIT FOR SOME TIME */
	LCD_cmd(0x02);								/* send for 4 bit initialization of LCD  */
	LCD_cmd(0x28);								/* INIT. LCD 2 LINE, 5 X 8 MATRIX 4bit mode */
	LCD_cmd(0x0C);								/* DISPLAY ON CURSOR ON */
	LCD_cmd(0x01);								/* LCD CLEAR */
	LCD_cmd(0x82);								/* SHIFT CURSOR TO WRITE */
}



//This function accepts a character array and passes it to LCD_Char() function to display a character at a time on the LCD.to display a character at a time on the LCD.
void LCD_Write_String(char *str)							/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)							/* Send each char of string till the NULL */
	{
		LCD_write (str[i]);							/* Call LCD data write */
	}
}

void LCD_Set()
{
	DDRB = 0xFF; //Set LCD data port as output
	DDRD = 0xE0; //Set LCD signals (RS,RW,E) as output
	LCD_init_LCD();  //Initialize LCD
	_delay_ms(10); // delay of 10 Milli seconds
	LCD_cmd(0x0C); //display on, cursor off
	_delay_ms(10);
}

