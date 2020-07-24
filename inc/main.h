#ifndef _LIBS_H_
#define _LIBS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "digitalio_avr.h"
#include "lcd.h"
#include "time.h"
#include "eeprom.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif


//*****************************************************************************
//	Variables which can be changed by "user"-----------------------------------
//*****************************************************************************

#define BUTTON_BEEP_DELAY	20		// duration of beep for buttons in 
									// miliseconds (max = 65535)

#define FINISH_BEEP_DELAY	1500	// duration of beep at the end of counting 
									// in miliseconds (max = 65535)

#define NUM_OF_CELLS 32				// number of timers, which can be reminded
									// by this device (max = 255)

//*****************************************************************************

#define FALSE 	0
#define TRUE 	1

#define BUZER_PORT &PORTD
#define BUZER_PIN 7

#define RELAY_PORT &PORTD
#define RELAY_PIN 6

enum enumStateOfRelay{
	RELAY_OFF = 0,
	RELAY_ON,
	RELAY_PAUSE
};

void EncoderCheck();

inline void ChangeValue(uint8_t * value, uint8_t min, uint8_t max, int16_t delta);

void beep(uint16_t BuzerDelay);

void init();

/*_LIBS_H_*/
#endif