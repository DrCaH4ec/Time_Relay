#ifndef _LCD_H_
#define _LCD_H_

//you can change it if you need
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#include "digitalio_avr.h"

//-----------------------------------------------------------------------------
#define ON 	1
#define OFF 	0
//-----------------------------------------------------------------------------
#define CLEAR_DISPLAY 			(1<<0)
#define RETURN_HOME 			(1<<1)
#define ENTRY_MODE_SET			(1<<2)
#define DISPLAY_ON_OFF_CONTROL		(1<<3)
#define CURSOR_OR_DISPLAY_SHIFT		(1<<4)
#define FUNCTION_SET			(1<<5)
#define SET_CGRAM_ADDRES		(1<<6)
#define SET_DDRAM_ADDRES		(1<<7)

// Entry mode set
#define SH 				(1<<0)
#define I_D				(1<<1)

// Display ON/OFF control
#define B 				(1<<0)
#define C 				(1<<1)
#define D 				(1<<2)

// Cursor or Display Shift
#define R_L				(1<<2)
#define S_C 				(1<<3)

// Function set
#define F 				(1<<2)
#define N				(1<<3)
#define DL 				(1<<4)

//-----------------------------------------------------------------------------
struct lcd_st{
	volatile uint8_t * RsPort;
	uint8_t RsPin;
	volatile uint8_t * EnablePort;
	uint8_t EnablePin;
	volatile uint8_t * D4Port;
	uint8_t D4Pin;
	volatile uint8_t * D5Port;
	uint8_t D5Pin;
	volatile uint8_t * D6Port;
	uint8_t D6Pin;
	volatile uint8_t * D7Port;
	uint8_t D7Pin;
};
//-----------------------------------------------------------------------------

/* You can use it for simplification

	struct lcd_st disp;

	disp.RsPort = 	;
	disp.RsPin = 	;

	disp.EnablePort = ;
	disp.EnablePin = ;

	disp.D7Port = 	;
	disp.D7Pin = 	;

	disp.D6Port = 	;
	disp.D6Pin = 	;

	disp.D5Port = 	;
	disp.D5Pin = 	;

	disp.D4Port = 	;
	disp.D4Pin = 	;

 */

//-----------------------------------------------------------------------------

/**
 * This function is used for initialisation of display. It initialises lcd in
 * 4-bit mode.
 * 
 * @param lcd        pointer to struct with pins to which LCD is connected
 * @param NumOfLines number of lines of current LCD
 */
void lcd_init(struct lcd_st * lcd, uint8_t NumOfLines);

/**
 * Clear display and set cursor to position (0, 0)
 * @param lcd pointer to struct with pins to which LCD is connected
 */
void lcd_Clear(struct lcd_st * lcd);

/**
 * Set cursor to position (0, 0) without changing data on display
 * @param lcd pointer to struct with pins to which LCD is connected
 */
void lcd_Home(struct lcd_st * lcd);

/**
 * Set cursor to position (x, y) without changing data on display
 * @param lcd pointer to struct with pins to which LCD is connected
 * @param x   new horisontal position of cursor on display
 * @param y   new vertical position of cursor on display
 */
void lcd_GoToXY(struct lcd_st * lcd, uint8_t x, uint8_t y);

/**
 * Print a string to display
 * @param lcd pointer to struct with pins to which LCD is connected
 * @param data pointer to first element of string which will be
 * printed do display
 */
void lcd_Puts(struct lcd_st * lcd, char data[]);

/**
 * Print only 1 character to display
 * @param lcd  pointer to struct with pins to which LCD is connected
 * @param data ASCII code of character or addres of custom character
 * in CGRAM
 */
void lcd_PutChar(struct lcd_st * lcd, char data);

/**
 * Makes corsor visible on display
 * @param lcd  pointer to struct with pins to which LCD is connected
 * @param mode if mode = 1 cursor is visible, else cursor is invisible
 */
void lcd_Cursor(struct lcd_st * lcd, bool mode);

/**
 * Makes corsor blinking on display
 * @param lcd  pointer to struct with pins to which LCD is connected
 * @param mode if mode = 1 cursor is blinking, else cursor isn't blinking
 */
void lcd_CursorBlink(struct lcd_st * lcd, bool mode);

/**
 * Shift all symbols on display on 1 position to left
 * @param lcd pointer to struct with pins to which LCD is connected
 */
void lcd_ShiftDisplayLeft(struct lcd_st * lcd);

/**
 * Shift all symbols on display on 1 position to right
 * @param lcd pointer to struct with pins to which LCD is connected
 */
void lcd_ShiftDisplayRight(struct lcd_st * lcd);

/**
 * Write to CGRAM custom character with size 5x8 dots
 * @param lcd      pointer to struct with pins to which LCD is connected
 * @param data     pointer to array with definition of new character
 * @param location addres of new character in CGRAM(can be from 0 to 7)
 */
void lcd_CreateChar(struct lcd_st * lcd, uint8_t data[], uint8_t location);


/*_LCD_H_*/
#endif