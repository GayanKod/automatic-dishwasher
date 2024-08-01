/*
 * Dishf.c
 *
 * Created: 11/17/2020 10:17:04 PM
 * Author : GayanKod
 */ 

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include "keypad header.h"
#include "ds18b20.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define KEY_PRT PORTA
#define KEY_DDR DDRA
#define KEY_PIN PINA

#define Relay01 PC2 //WashPump
#define Relay02 PC1 //Heater
#define Relay03 PC0 //Solenoid for Wash Pump
#define Relay04 PC3 //Solenoid for Drain Pump


void NormalMode();
void LongMode();
void FastMode();
void FruitMode();


int GetTemp();
void WaterIn();
void Drying(int dryT);
void HotWaterWashing(int WashT, int tm);
void NormalWashing(int tm);
void WaterDraining();
int GetWaterLevel();

int extraTime=0;


int main(void)
{
	DDRC = 0xFF;
	DDRA = 0x00;
	DDRD = 0x00;
	
    while(1)
    {
	    LCD_Set();
		    
		    while(1)
		    {
				LCD_Write_String("Enter a mode");
				LCD_cmd(0xC0);
				
			    char test = keyfind();
				
			    LCD_write(test);
			    switch(test){
				    case '1':
						NormalMode();
						break;
				    case '2':
						LongMode();
						break;
				    case '3':
						FastMode();
						break;
				    case '4':
						FruitMode();
						break;
				    default:
				    LCD_Write_String(" Wrong input");
			    }
		    }
		    LCD_cmd(0x01);
	    }
	    return 0;
}

void NormalMode(){

		LCD_cmd(0x80);
		LCD_Write_String(" Normal Mode ");
		LCD_cmd(0xC0);
		LCD_Write_String(" Enabled ");
		_delay_ms(500);
	
		//Wash Time = 15 seconds for each cycle
		int NoOfcycle=4;
		int washTemp=60;
		int dryTemp=80;
		
	
		while (GetWaterLevel()==0){
			LCD_cmd(0x01);
			LCD_cmd(0x80);
			LCD_Write_String(" Water Lvl Low ");
			_delay_ms(500);
			LCD_cmd(0xC0);
			LCD_Write_String(" Water In... ");
			
			WaterIn();
			_delay_ms(500);
			LCD_cmd(0x01);
		}
		
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String(" Water Lvl High ");
		_delay_ms(200);
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String("Wash Processing");
		_delay_ms(200);
		
		HotWaterWashing(washTemp,2);
		_delay_ms(200);
		WaterDraining();
		
		for(int i=0; i<NoOfcycle; i++){
			WaterIn();
			NormalWashing(2); //3x7 cycle minimize to 3x5 //timer set to 15seconds wash time
			WaterDraining();
		}
		
		Drying(dryTemp);
		_delay_ms(200);
		WaterDraining();
		_delay_ms(200);
}

void LongMode(){

	LCD_cmd(0x80);
	LCD_Write_String(" Long Mode ");
	LCD_cmd(0xC0);
	LCD_Write_String(" Enabled ");
	_delay_ms(500);
	
	//Wash Time = 15 seconds for each cycle
	int NoOfcycle=6;
	int washTemp=60;
	int dryTemp=80;
	
	while (GetWaterLevel()==0){
		
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String(" Water Lvl Low ");
		_delay_ms(200);
		LCD_cmd(0xC0);
		LCD_Write_String(" Water In... ");
		_delay_ms(200);
		
		WaterIn();
		_delay_ms(500);
		LCD_cmd(0x01);
		
	}
		
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String(" Water Lvl High ");
		_delay_ms(200);
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String("Wash Processing");
		_delay_ms(200);
		
		HotWaterWashing(washTemp,2);
		_delay_ms(500);
		
		for(int i=0; i<NoOfcycle; i++){
			WaterIn();
			NormalWashing(2); //3x7 cycle minimize to 3x5 //timer set to 15seconds wash time
			WaterDraining();
		}
	
	Drying(dryTemp);
	_delay_ms(200);
	WaterDraining();
	_delay_ms(200);
}

void FastMode(){

	LCD_cmd(0x80);
	LCD_Write_String(" Fast Mode ");
	_delay_ms(200);
	LCD_cmd(0xC0);
	LCD_Write_String(" Enabled ");
	_delay_ms(200);
	
	//Wash Time = 12 Seconds for each cycle
	int NoOfcycle=3;
	
	
	while(GetWaterLevel()==0){
		
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String(" Water Lvl Low ");
		_delay_ms(200);
		LCD_cmd(0xC0);
		LCD_Write_String(" Water In... ");
		_delay_ms(200);
		
		WaterIn();
		_delay_ms(500);
		LCD_cmd(0x01);
		
	}
		
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String(" Water Lvl High ");
		_delay_ms(200);
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String("Wash Processing");
		_delay_ms(200);
		
		for(int i=0; i<NoOfcycle; i++){
			WaterIn();
			NormalWashing(3); //3x7 cycle minimize to 3x4 //timer set to 12seconds wash time
			WaterDraining();
		}
}

void FruitMode(){
	
	LCD_cmd(0x80);
	LCD_Write_String(" Fruit Mode ");
	_delay_ms(200);
	LCD_cmd(0xC0);
	LCD_Write_String(" Enabled ");
	_delay_ms(200);
	
	//wash time=15 seconds for each cycles
	int NoOfcycle=2;
	
	if (GetWaterLevel()==0){
		
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_Write_String(" Water Lvl Low ");
		_delay_ms(200);
		LCD_cmd(0xC0);
		LCD_Write_String(" Water In... ");
		_delay_ms(200);
		
		WaterIn();
		_delay_ms(500);
		LCD_cmd(0x01);
		
	}
	
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Water Lvl High ");
	_delay_ms(200);
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String("Wash Processing");
	_delay_ms(200);
	
	for(int i=0; i<NoOfcycle; i++){
		WaterIn();
		NormalWashing(2); //3x7 cycle minimize to 3x5 //timer set to 15seconds wash time
		WaterDraining();
	}
}




int GetWaterLevel(){
		
		while(1){
		
		if (PIND & (1<<PIND1)){ //if Pin1 of Port A is HIGH //Water level Sensor Float switch Small PP (MD0376)
			 return(0);
		}else
			return(1);
	}
}


int GetTemp(){
	
	int temp;
	
	//Start conversion (without ROM matching)
	ds18b20convert( &PORTD, &DDRD, &PIND, ( 1 << 7 ), NULL );

	//Delay (sensor needs time to perform conversion)
	_delay_ms( 100 );

	//Read temperature (without ROM matching)
	ds18b20read( &PORTD, &DDRD, &PIND, ( 1 << 7 ), NULL, &temp );

	temp= temp/16;   //covert reading to celcius
	
	return(temp);	

	
}

void Drying(int dryT){
	
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" WaterIn ");
	_delay_ms(200);
	WaterIn();
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Drying... ");
	_delay_ms(200);
	
	
	while (GetTemp()<dryT){
		
		PORTC = (1<<Relay02);; //Turn On Relay //Turn on Heater
	}
	
	if (GetTemp()<=dryT+5 && GetTemp()>=dryT-5){
		PORTC = (1<<Relay02);; //Turn Off Relay //Turn Off Heater
	}
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Drying Finished ");
	_delay_ms(200);
}


void HotWaterWashing(int WashT, int tm){
	
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String("Wtr Heats Up...");
	_delay_ms(500);
	
	while (GetTemp()<WashT){
		
		PORTC = (1<<Relay02);; //Turn On Relay //Turn on Heater
		_delay_ms(1000);
	}
	
	if (GetTemp()<=WashT+5 && GetTemp()>=WashT-5){
		PORTC = (0<<Relay02);; //Turn Off Relay //Turn Off Heater
	}
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String("Heating Finished");
	_delay_ms(1000);
	
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Washing... ");
	_delay_ms(500);
	
	PORTC = (1<<Relay01);; //Turn On Relay //Turn On Sprinkler
	_delay_ms(1500);
	
	//AddTimer
	extraTime=tm;
	TCCR1B = (1 << WGM12); //Timer
	OCR1A = 46875;//Ticks for 3seconds
	TIMSK = (1 << OCIE1A);
	
	sei();
	
	TCCR1B |= (1 << CS12) | (1 << CS10); //1024 Pre-scaler
	//End Timer
	
	_delay_ms(200);
	PORTC = (0<<Relay01);; //Turn off Relay //Turn off sprinkler
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Finished ");
	LCD_cmd(0x01);
	LCD_cmd(0x80);
}

void NormalWashing(int tm){
	
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Washing... ");
	_delay_ms(200);
	
	PORTC = (1<<Relay01);; //Turn On Relay //Turn On Wash Pump
	_delay_ms(1500);
	
	//AddTimer
	extraTime=tm;
	TCCR1B = (1 << WGM12); //Timer
	OCR1A = 46875;//Ticks for 3seconds
	TIMSK = (1 << OCIE1A);
	
	sei();
	
	TCCR1B |= (1 << CS12) | (1 << CS10); //1024 Pre-scaler
	//End Timer
	
	_delay_ms(200);
	PORTC = (0<<Relay01);; //Turn off Relay //Turn off sprinkler
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Finished ");
	LCD_cmd(0x01);
}

void WaterDraining(){
	
	//Water Draining
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Draining... ");
	_delay_ms(200);
	
	while(GetWaterLevel()==1){
		PORTC = (1<<Relay04);; //Turn On Relay04 //Turn On Solenoid for Drain Pump & Drain Pump
	}
	PORTC= (0<<Relay04);; //Turn Off Relay04
	
	LCD_cmd(0x01);
}

void WaterIn(){
	
	//Water In
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_Write_String(" Water In... ");
	_delay_ms(200);
	
	while(GetWaterLevel()==0){
		PORTC = (1<<Relay03);; //Turn On Relay03 //Turn On Solenoid for Drain Pump & Drain Pump
	}
	PORTC= (0<<Relay03);; //Turn Off Relay03

}

ISR(TIMER1_COMPA_vect){
	
	extraTime++;
	if (extraTime>6){
		extraTime=0;
		PORTC = (0<<Relay01);; //Turn Off Relay //Turn Off Wash Pump after washing
	}
}
