#ifndef _EEPROM_H_
#define _EEPROM_H_


#include <avr/io.h>
#include <stdint.h>


void EEPROM_write(uint8_t uiAddress, uint8_t ucData);

uint8_t EEPROM_read(uint8_t uiAddress);

/*_EEPROM_H_*/
#endif
