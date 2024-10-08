/*
 SRIO.cpp - Library for shift registers 74HC165/74HC595 IN/OUT.
 Created by e-licktronic for CD4021
 modified by Julien Voirin 4colors@free.fr for 74HC165
 Released into the public domain.
 For Arduino Mega 2560
 */

#include "Arduino.h"
#include "SRIO_mxctr.h"

SRIO SR;

void SRIO:: Initialize()
{
	pinMode(_latchPinIn,  OUTPUT);
	pinMode(_clockPin,    OUTPUT);
    pinMode(_dataPinIn,   INPUT);
	pinMode(_latchPinOut, OUTPUT);
	pinMode(_dataPinOut,  OUTPUT);
	
	byte _SR[8] = {0};
}

uint8_t SRIO::shiftIn_SRIO() 
{
	int i;
//	int temp = 0;
//    int pinState;
	byte myDataIn = 0;
	
//	for (i=7; i>=0; i--)
	for (i=0; i<8; i++) // INCREMENT rather decrement
	{
		PORTH &= 0 << 6; // ClockPin 9 low
//		delayMicroseconds(2);
		__asm__("nop\n\t");

//		temp = PINB; // dataPinIn 11
//		if (bitRead (temp, 5)) {
		if (bitRead (PINB, 5)) {
//			pinState = 1;
			myDataIn = myDataIn | (1 << i);
		}
		else {
//			pinState = 0;
		}
		PORTH |= 1 << 6; // ClockPin 9 high
		

	}
	return myDataIn;
/*
	byte value = 0;
	
	for (byte i = 0; i <8; i++) {
		value = value | (bitRead(PINB,6)<<(7-i));//value |  (digitalRead(6) << (7-i));
		PORTB |= (1<<7);//digitalWrite(7, HIGH);
		PORTB &= ~(1<<7);// digitalWrite(7, LOW);
	}
	return value;
	*/
}

void SRIO::shiftOut_SRIO(uint8_t val)
{
	uint8_t i;
	
	for (i = 0; i < 8; i++)  {
		//PORTB |= !!(val & (1 << (7 - i)))<<7; //dataPinOut 13
		bitWrite(PORTB, 7, !!(val & (1 << (7 - i))) );
		PORTH |= 1 << 6; //clockPin 9 high
        PORTH &= ~1 << 6; //clockPin 9 low
	}
}

void SRIO::Led_All_On()
{
	//Allume toutes les Leds
	PORTB &= 0<<6; //latchPinOut 12 low
	shiftOut_SRIO(255);
	shiftOut_SRIO(255);
	shiftOut_SRIO(255);
	shiftOut_SRIO(255);    
	shiftOut_SRIO(255);
	shiftOut_SRIO(255);
	shiftOut_SRIO(255);
	shiftOut_SRIO(255);
	PORTB |= 1 << 6; //latchPinOut 12 high
	for(byte i=0; i<8; i++)
	{
		_SR[i]=255;
	}
}

void SRIO::Led_All_Off()
{
	//Eteint toutes les Leds
	PORTB &= 0<<6; //latchPinOut 12 low
	shiftOut_SRIO(0);
	shiftOut_SRIO(0);
	shiftOut_SRIO(0);
	shiftOut_SRIO(0);    
	shiftOut_SRIO(0);
	shiftOut_SRIO(0);
	shiftOut_SRIO(0);
	shiftOut_SRIO(0);
	PORTB |= 1 << 6; //latchPinOut 12 high
	for(byte i=0; i<8; i++)
	{
		_SR[i]=0;
	}
}

void SRIO::Led_SR_Write (byte SR_num, byte data)
{
	// Fonction qui envoie un octet au SR selectionn�
	_SR[SR_num]=data;
	PORTB &= 0 << 6;//latchPinOut 12 low
	shiftOut_SRIO(_SR[7]);
	shiftOut_SRIO(_SR[6]);
	shiftOut_SRIO(_SR[5]);
	shiftOut_SRIO(_SR[4]);
	shiftOut_SRIO(_SR[3]);
	shiftOut_SRIO(_SR[2]);
	shiftOut_SRIO(_SR[1]);
	shiftOut_SRIO(_SR[0]);  
	PORTB |= 1<<6;//latchPinOut 12 high
} 

byte SRIO::Led_SR_Read (byte SR_num)
{ 
	// Fonction qui renvoi l'�tat de l'octet du SR s�lectionn�
	return _SR[SR_num];
}

byte SRIO::Led_Pin_Read (byte Pin_num)
{
    // Fonction qui renvoi l'�tat de l'octet du SR s�lectionn�
    return bitRead(_SR[Pin_num / 8], Pin_num - (8 * (Pin_num / 8)));
    //	return flag;
    
}
void SRIO::Led_Pin_Write (byte Pin_num, byte flag)
{
	// Fonction qui envoi l'�tat de le led  byte data;
	bitWrite(_SR[Pin_num/8],Pin_num-(8*(Pin_num/8)),flag);
	PORTB &= 0 << 6;//digitalWrite(_latchPinOut, 0);
	shiftOut_SRIO(_SR[7]);
	shiftOut_SRIO(_SR[6]);
	shiftOut_SRIO(_SR[5]);
	shiftOut_SRIO(_SR[4]);
	shiftOut_SRIO(_SR[3]);
	shiftOut_SRIO(_SR[2]);
	shiftOut_SRIO(_SR[1]);
	shiftOut_SRIO(_SR[0]);
	PORTB |= 1 << 6; //digitalWrite(_latchPinOut, 1);
}

byte SRIO::Button_SR_Read (byte SR_num)
{
	// Fonction qui renvoie sous forme d'octet l'�tat des boutons du SR s�lectionn�
//	digitalWrite(_latchPinIn, 1); 
 
//	delayMicroseconds(2);
//	__asm__("nop\n\t""nop\n\t""nop\n\t");
//	digitalWrite(_latchPinIn, 0);
	digitalWrite(_latchPinIn, 1); // CHANGED for 74hc165
	
	_data[0] = shiftIn_SRIO();
	_data[1] = shiftIn_SRIO();
	_data[2] = shiftIn_SRIO();
	_data[3] = shiftIn_SRIO();
	_data[4] = shiftIn_SRIO();
	_data[5] = shiftIn_SRIO();
	_data[6] = shiftIn_SRIO();
	_data[7] = shiftIn_SRIO();
	
	digitalWrite(_latchPinIn, 0); // CHANGED for 74hc165
	
	return _data[SR_num];
}

byte SRIO::Button_Pin_Read (byte Pin_num){
	// fonction qui permet de tester une PIN, renvoie 1 ou 0
	boolean flag = 0;
	//	digitalWrite(_latchPinIn, 1); 
//	digitalWrite(_latchPinIn, 0); // CHANGED for 74hc165
//	delayMicroseconds(2);

	//__asm__("nop\n\t""nop\n\t""nop\n\t");

	//	digitalWrite(_latchPinIn, 0);
	digitalWrite(_latchPinIn, 1); // CHANGED for 74hc165


	_data[0] = shiftIn_SRIO();
	_data[1] = shiftIn_SRIO();
	_data[2] = shiftIn_SRIO();
	_data[3] = shiftIn_SRIO();
	_data[4] = shiftIn_SRIO();
	_data[5] = shiftIn_SRIO();
	_data[6] = shiftIn_SRIO();
	_data[7] = shiftIn_SRIO();
	
	digitalWrite(_latchPinIn, 0); // CHANGED for 74hc165
	
    if (bitRead (_data[Pin_num/8], Pin_num-(8*(Pin_num/8)) )){
//		flag = 1;
		flag = 0; // CHANGED -> RELEASED = 0
    }
    else{
//		flag = 0;	
		flag = 1; // CHANGED -> PRESSED = 1
    }
	return flag;
}
