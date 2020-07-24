#Makefile for building code for avr microcontrollers

#------------------------------------------------------------------------------
# ------------------------YOU CAN(and NEED) TO CHANGE IT-----------------------
#------------------------------------------------------------------------------
MCU = atmega328p
TARGET = relay
#------------------------------------------------------------------------------
PROGRAMMER = arduino
SPEED = 57600

PORT = COM3

MEMTYPE = flash
	
#you need to contain all you *.h files in dir "inc" IT'S A RULE
INC_DIR = inc
#flags for compiler
WFLAGS = -Wall -Werror
#level of optimisation
OPTIM = -O2 -Os



#------------------------------------------------------------------------------
src_files := $(wildcard *.c)
obj_files := $(src_files:.c=.o)

#------------------------------------------------------------------------------
%.o: %.c
	avr-gcc $(WFLAGS) $(OPTIM) -I $(INC_DIR) -mmcu=$(MCU) -c $< -o $@

#------------------------------------------------------------------------------
$(TARGET).elf: $(obj_files)
	avr-gcc -mmcu=$(MCU) -o $@ $^

#------------------------------------------------------------------------------
build: $(TARGET).elf
	avr-objcopy -j .text -j .data -O ihex $^ $(TARGET).hex

#------------------------------------------------------------------------------
.PHONY: clean
clean:
	del -f $(obj_files) $(TARGET).elf *.gch

#------------------------------------------------------------------------------
.PHONY: tidy
tidy: clean
	del -f $(TARGET).hex

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
.PHONY: prog
prog:
	avrdude -p $(MCU) -b $(SPEED) -c $(PROGRAMMER) -P $(PORT) -U $(MEMTYPE):w:$(TARGET).hex

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
.PHONY: read_eeprom
read_eeprom:
	avrdude -p $(MCU) -b $(SPEED) -c $(PROGRAMMER) -P $(PORT) -U eeprom:r:eeprom.hex:i
		