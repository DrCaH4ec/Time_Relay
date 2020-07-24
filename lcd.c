#include "lcd.h"


//#############################################################################
//#####################---INTERNAL FUNCTIONS---################################
//#############################################################################

void PulseEnable(struct lcd_st * lcd)
{
	DigitalWrite(lcd->EnablePort, lcd->EnablePin, LOW);
	_delay_us(10);
	DigitalWrite(lcd->EnablePort, lcd->EnablePin, HIGH);
	_delay_us(10);
	DigitalWrite(lcd->EnablePort, lcd->EnablePin, LOW);
	_delay_us(100);
}

//-----------------------------------------------------------------------------

// part=1 -> high
// part=0 -> low
void SendHalfByte(struct lcd_st * lcd, uint8_t data, bool part)
{
	if(part){
		DigitalWrite(lcd->D4Port, lcd->D4Pin, data&(1<<4));
		DigitalWrite(lcd->D5Port, lcd->D5Pin, data&(1<<5));
		DigitalWrite(lcd->D6Port, lcd->D6Pin, data&(1<<6));
		DigitalWrite(lcd->D7Port, lcd->D7Pin, data&(1<<7));		
	}
	else{
		DigitalWrite(lcd->D4Port, lcd->D4Pin, data&(1<<0));
		DigitalWrite(lcd->D5Port, lcd->D5Pin, data&(1<<1));
		DigitalWrite(lcd->D6Port, lcd->D6Pin, data&(1<<2));
		DigitalWrite(lcd->D7Port, lcd->D7Pin, data&(1<<3));		
	}

	PulseEnable(lcd);
}

//-----------------------------------------------------------------------------

void SendInstruction(struct lcd_st * lcd, uint8_t data)
{
	DigitalWrite(lcd->RsPort, lcd->RsPin, LOW);

	SendHalfByte(lcd, data, HIGH);
	SendHalfByte(lcd, data, LOW);	
}

//-----------------------------------------------------------------------------

void SendData(struct lcd_st * lcd, uint8_t data)
{
	DigitalWrite(lcd->RsPort, lcd->RsPin, HIGH);
	_delay_us(1);

	SendHalfByte(lcd, data, HIGH);
	SendHalfByte(lcd, data, LOW);

	_delay_us(50);
}

//#############################################################################
//#####################---EXTERNAL FUNCTIONS---################################
//#############################################################################

void lcd_Clear(struct lcd_st * lcd)
{
	SendInstruction(lcd, CLEAR_DISPLAY);
	_delay_ms(2);
}

//-----------------------------------------------------------------------------

void lcd_Home(struct lcd_st * lcd)
{
	SendInstruction(lcd, RETURN_HOME);
	_delay_ms(2);
}

//-----------------------------------------------------------------------------

void lcd_init(struct lcd_st * lcd, uint8_t NumOfLines)
{
	uint8_t tmp = 0x00;

	DigitalWrite(lcd->RsPort, lcd->RsPin, LOW);
	DigitalWrite(lcd->EnablePort, lcd->EnablePin, LOW);
	DigitalWrite(lcd->D4Port, lcd->D4Pin, LOW);
	DigitalWrite(lcd->D5Port, lcd->D5Pin, LOW);
	DigitalWrite(lcd->D6Port, lcd->D6Pin, LOW);
	DigitalWrite(lcd->D7Port, lcd->D7Pin, LOW);

	_delay_ms(50);

	tmp = FUNCTION_SET | DL;

	SendHalfByte(lcd, tmp, HIGH);
	_delay_ms(5);

	SendHalfByte(lcd, tmp, HIGH);
	_delay_us(150);

	SendHalfByte(lcd, tmp, HIGH);

	_delay_us(50);

	tmp = 0x00;
	tmp |= FUNCTION_SET;
	SendHalfByte(lcd, tmp, HIGH);

	//2 lines mode set
	//5x8 dots
	tmp = NumOfLines > 1 ? (tmp | N) : tmp;
	SendInstruction(lcd, tmp);

	SendInstruction(lcd, tmp);

	tmp = 0x00;
	tmp = DISPLAY_ON_OFF_CONTROL | D;
	SendInstruction(lcd, tmp);

	tmp = 0x00;
	tmp = ENTRY_MODE_SET | I_D;
	SendInstruction(lcd, tmp);

	lcd_Clear(lcd);	
}

//-----------------------------------------------------------------------------

void lcd_GoToXY(struct lcd_st * lcd, uint8_t x, uint8_t y)
{
	uint8_t tmp = (0x40 * y + x) | SET_DDRAM_ADDRES;

	SendInstruction(lcd, tmp);
	_delay_us(100);
}

//-----------------------------------------------------------------------------

void lcd_Puts(struct lcd_st * lcd, char data[])
{
	uint8_t i = 0;

	while(data[i] != '\0' && i < 16){
		SendData(lcd, data[i]);
		i++;
	}
}

//-----------------------------------------------------------------------------

void lcd_PutChar(struct lcd_st * lcd, char data)
{
	SendData(lcd, data);
}

//-----------------------------------------------------------------------------

void lcd_Cursor(struct lcd_st * lcd, bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL | D;

	tmp = mode ? (tmp | C) : tmp;

	SendInstruction(lcd, tmp);
}

//-----------------------------------------------------------------------------

void lcd_CursorBlink(struct lcd_st * lcd, bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL | D;

	tmp = mode ? (tmp | B) : tmp;

	SendInstruction(lcd, tmp);
}

//-----------------------------------------------------------------------------

void lcd_Display(struct lcd_st * lcd, bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL;

	tmp = mode ? (tmp | D) : tmp;

	SendInstruction(lcd, tmp);
}

//-----------------------------------------------------------------------------

void lcd_ShiftDisplayLeft(struct lcd_st * lcd)
{
	uint8_t tmp = CURSOR_OR_DISPLAY_SHIFT | S_C;

	SendInstruction(lcd, tmp);
}

//-----------------------------------------------------------------------------

void lcd_ShiftDisplayRight(struct lcd_st * lcd)
{
	uint8_t tmp = CURSOR_OR_DISPLAY_SHIFT | S_C | R_L;

	SendInstruction(lcd, tmp);
}

//-----------------------------------------------------------------------------

void lcd_CreateChar(struct lcd_st * lcd, uint8_t data[], uint8_t location)
{
	uint8_t tmp = SET_CGRAM_ADDRES | (location<<3);
	SendInstruction(lcd, tmp);

	uint8_t i;
	for(i = 0; i < 8; i++)
		SendData(lcd, data[i]);
}
