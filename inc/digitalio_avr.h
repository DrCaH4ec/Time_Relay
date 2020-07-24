#ifndef _DIGITAL_IO_AVR_H_
#define _DIGITAL_IO_AVR_H_

//-----------------------------------------------------------------------------
#define INPUT		0
#define OUTPUT 		1

//here you need to use &DDRx (yeah, I use pointers in this case)
#define PinMode(ddr, pin, state) *ddr = state ? *ddr|(1<<pin) : *ddr&(~(1<<pin))
//-----------------------------------------------------------------------------
#define HIGH	1
#define LOW	0
#define PULL_UP 1	//for input mode only

//here you need to use &PORTx (yeah, I use pointers in this case)
#define DigitalWrite(port, pin, val) *port = val ? *port|(1<<pin) : *port&(~(1<<pin))
//-----------------------------------------------------------------------------

//here you need to use &PINx (yeah, I use pointers in this case)
#define DigitalRead(port, pin) *port&(1<<pin)
//-----------------------------------------------------------------------------

/*_DIGITAL_IO_AVR_H_*/
#endif