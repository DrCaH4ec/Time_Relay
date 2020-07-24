#include "inc/main.h"

//*****************************************************************************
//	Global variables-----------------------------------------------------------
//*****************************************************************************

// Encoder's variables
volatile int16_t EncDelta = 0;
volatile bool EncFlag = FALSE;

// Screen's variables
volatile bool ArrowPosition = FALSE;
volatile bool UpdateScreen = TRUE;

// Relay's state variables
volatile bool Pause = FALSE;
enum enumStateOfRelay StateOfRelay = RELAY_OFF;

// Timer's variables
struct Time_st MainCnt = {0, 0, 0};
uint8_t NumOfTimer = 0;
struct Time_st SettedTime[NUM_OF_CELLS];

// Buzer's variable
volatile uint16_t BuzerCnt = 0;


//*****************************************************************************
//	Interrupt every 1us--------------------------------------------------------
//*****************************************************************************
ISR(TIMER0_COMPA_vect)
{
	static uint16_t cnt = 0;
	cli();

	EncoderCheck();

//####################

	if(StateOfRelay == RELAY_ON){
		if(cnt < 1000){
			cnt++;
		}
		else{
			cnt = 0;
			Time_Dec(&MainCnt);
			UpdateScreen = TRUE;
		}
	}

	if(MainCnt.min == 0 && MainCnt.sec == 0 && StateOfRelay == RELAY_ON){
		StateOfRelay = RELAY_OFF;
		UpdateScreen = TRUE;
		beep(FINISH_BEEP_DELAY);
	}

//####################

	if(BuzerCnt > 0)
		BuzerCnt--;
	else
		DigitalWrite(BUZER_PORT, BUZER_PIN, LOW);

	sei();
}

//*****************************************************************************
//	Extern interrupt for start/stop button-------------------------------------
//*****************************************************************************
ISR(INT0_vect)
{
	cli();

		UpdateScreen = TRUE;

		beep(BUTTON_BEEP_DELAY);

//####################

		if(StateOfRelay != RELAY_ON)
			StateOfRelay = RELAY_ON;
		else
			StateOfRelay = RELAY_PAUSE;

//####################

		if(MainCnt.min == 0 && MainCnt.sec == 0){
			MainCnt.min = SettedTime[NumOfTimer].min;
			MainCnt.sec = SettedTime[NumOfTimer].sec;
		}

	sei();
}

//*****************************************************************************
//	Extern interrupt for encoder's button--------------------------------------
//*****************************************************************************
ISR(INT1_vect)
{
	cli();

		beep(BUTTON_BEEP_DELAY);

//####################

		UpdateScreen = TRUE;
		if(ArrowPosition)
			ArrowPosition = FALSE;
		else
			ArrowPosition = TRUE;

	sei();
}

//*****************************************************************************
//	Main void------------------------------------------------------------------
//*****************************************************************************
int main(void)
{
//-----------------------------------------------
// Local variables
//-----------------------------------------------

	uint8_t arrow[8] = {
		0b00000,
		0b00100,
		0b00010,
		0b11111,
		0b00010,
		0b00100,
		0b00000,
		0b00000
	};

	struct lcd_st disp;

	disp.RsPort = 	&PORTC;
	disp.RsPin = 	0;

	disp.EnablePort = &PORTC;
	disp.EnablePin = 1;

	disp.D7Port = 	&PORTC;
	disp.D7Pin = 	5;

	disp.D6Port = 	&PORTC;
	disp.D6Pin = 	4;

	disp.D5Port = 	&PORTC;
	disp.D5Pin = 	3;

	disp.D4Port = 	&PORTC;
	disp.D4Pin = 	2;

	uint8_t i = 0;
	char buff[17];

	SettedTime[0].min = 0;
	SettedTime[0].sec = 10;	

	for(i = 0; i < NUM_OF_CELLS; i++)
		SettedTime[i].hour = 0;

	init();
	lcd_init(&disp, 2);
	lcd_CreateChar(&disp, arrow, 1);

	// for(i = 0; i < NUM_OF_CELLS*2; i++){
	// 	EEPROM_write(i, 0);
	// };

	for(i = 0; i < NUM_OF_CELLS; i++){
		SettedTime[i].min = EEPROM_read(i);
		SettedTime[i].sec = EEPROM_read(i+NUM_OF_CELLS);
	};


	beep(500);

	sei();

	while(1){

		if(EncFlag && StateOfRelay != RELAY_ON){
			UpdateScreen = TRUE;

			if(!ArrowPosition){
				ChangeValue(&NumOfTimer, 0, NUM_OF_CELLS-1, EncDelta);

				if(NumOfTimer != 0 && EncDelta != 0 && StateOfRelay == RELAY_PAUSE){
					StateOfRelay = RELAY_OFF;
					Time_Reset(&MainCnt);
				}
			}
			else{

				if(StateOfRelay != RELAY_PAUSE){
					if(!((SettedTime[NumOfTimer].min == 0 && SettedTime[NumOfTimer].sec == 0 && EncDelta < 0) 
						&& (SettedTime[NumOfTimer].min >= 99 && EncDelta > 0))){
						if(EncDelta > 0)
							Time_Inc(&SettedTime[NumOfTimer]);
						else if(EncDelta < 0)
							Time_Dec(&SettedTime[NumOfTimer]);

						EEPROM_write(NumOfTimer, SettedTime[NumOfTimer].min);
						EEPROM_write(NumOfTimer + NUM_OF_CELLS, SettedTime[NumOfTimer].sec);
					}
				}
				else{
					if(!((MainCnt.min == 0 && MainCnt.sec == 0 && EncDelta < 0) 
						&& (MainCnt.min >= 99 && EncDelta > 0))){
						if(EncDelta > 0)
							Time_Inc(&MainCnt);
						else if(EncDelta < 0)
							Time_Dec(&MainCnt);					
					}
				}
			}

			EncFlag = FALSE;
		}

		if(StateOfRelay == RELAY_ON)
			DigitalWrite(RELAY_PORT, RELAY_PIN, HIGH);
		else
			DigitalWrite(RELAY_PORT, RELAY_PIN, LOW);


		if(UpdateScreen){	

			lcd_Clear(&disp);
			if(!ArrowPosition)
				lcd_GoToXY(&disp, 0, 0);
			else
				lcd_GoToXY(&disp, 0, 1);

			lcd_PutChar(&disp, 1);

			lcd_GoToXY(&disp, 1, 0);
			sprintf(buff, "Timer[%d]:", NumOfTimer);
			lcd_Puts(&disp, buff);

			if(StateOfRelay == RELAY_PAUSE){
				lcd_GoToXY(&disp, 15, 0);
				lcd_PutChar(&disp, 'P');
			}

			lcd_GoToXY(&disp, 1, 1);

			switch(StateOfRelay){
				case RELAY_ON:
					sprintf(buff, "%d%d:%d%d Relay ON", MainCnt.min/10, MainCnt.min%10, MainCnt.sec/10, MainCnt.sec%10);
				break;

				case RELAY_PAUSE:
					sprintf(buff, "%d%d:%d%d Relay OFF", MainCnt.min/10, MainCnt.min%10, MainCnt.sec/10, MainCnt.sec%10);
				break;

				case RELAY_OFF:
					sprintf(buff, "%d%d:%d%d Relay OFF", SettedTime[NumOfTimer].min/10, SettedTime[NumOfTimer].min%10, SettedTime[NumOfTimer].sec/10, SettedTime[NumOfTimer].sec%10);
				break;
			}

			lcd_Puts(&disp, buff);

			UpdateScreen = FALSE;
		}
	}

	return 0;
}



void EncoderCheck()
{
	static uint8_t UpState = 0;
	static uint8_t DownState = 0;
	static uint8_t OldState = 0;

	uint8_t NewState = PIND&(0b11<<4);

	NewState >>= 4;

	if(NewState != OldState){

		switch(OldState){
			case 0:
				if(NewState == 2) DownState++;
				if(NewState == 1) UpState++;
			break;

			case 1:
				if(NewState == 0) DownState++;
				if(NewState == 3) UpState++;
			break;

			case 2:
				if(NewState == 3) DownState++;
				if(NewState == 0) UpState++;
			break;

			case 3:
				if(NewState == 1) DownState++;
				if(NewState == 2) UpState++;
			break;
		}


		OldState = NewState;

	if(UpState >= 4){
		EncDelta++;
		UpState = 0;
		EncFlag = 1;
	}

	if(DownState >= 4){
		EncDelta--;
		DownState = 0;
		EncFlag = 1;
	}


	}
	else{
		EncFlag = 0;
	}
}


inline void ChangeValue(uint8_t * value, uint8_t min, uint8_t max, int16_t delta)
{
	if(!((*value == min && delta < 0) || (*value >= max && delta > 0)))
		*value += delta;
}


void beep(uint16_t BuzerDelay)
{
	if(BuzerCnt == 0){
		DigitalWrite(BUZER_PORT, BUZER_PIN, HIGH);
		BuzerCnt = BuzerDelay;
	}
}


void init()
{
//-----------------------------------------------
// Initialisation of PORTS
//-----------------------------------------------

// For LCD
	PinMode(&DDRC, 0, OUTPUT);	// RS
	PinMode(&DDRC, 1, OUTPUT);	// E
	PinMode(&DDRC, 2, OUTPUT);	// D4
	PinMode(&DDRC, 3, OUTPUT);	// D5
	PinMode(&DDRC, 4, OUTPUT);	// D6
	PinMode(&DDRC, 5, OUTPUT);	// D7

// For encoder
	PinMode(&DDRD, 3, INPUT);	// ENC_BUT
	PinMode(&DDRD, 4, INPUT);	// ENC_1
	PinMode(&DDRD, 5, INPUT);	// ENC_2

	DigitalWrite(&PORTD, 3, PULL_UP);
	DigitalWrite(&PORTD, 4, PULL_UP);
	DigitalWrite(&PORTD, 5, PULL_UP);

// For start/stop button
	PinMode(&DDRD, 2, INPUT);

	DigitalWrite(&PORTD, 2, PULL_UP);

// For buzer
	PinMode(&DDRD, 7, OUTPUT);

// For relay
	PinMode(&DDRD, 6, OUTPUT);


//-----------------------------------------------
// Initialisation of TIMER0:
// Interrupt on compare A match wit OCR0A
// Frequency: 1kHz
//-----------------------------------------------

	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS01) | (1<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	TCNT0 = 0;
	OCR0A = 249;


//-----------------------------------------------
// Initialisation of external interruptions
// INT0 -> enable | faling edge
// INT1 -> enable | faling edge
//-----------------------------------------------
	EICRA |= (1<<ISC01) | (1<<ISC11);
	EIMSK |= (1<<INT0) | (1<<INT1);
}
