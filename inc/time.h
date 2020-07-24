#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include <stdio.h>

struct Time_st{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
};

inline void Time_Inc(struct Time_st * my_time);

void Time_Dec(struct Time_st * my_time);

void Time_IncMin(struct Time_st * my_time);

void Time_DecMin(struct Time_st * my_time);

int8_t Time_Cmp(struct Time_st * my_time1, struct Time_st * my_time2);

inline void Time_Reset(struct Time_st * my_time);

/*_TIME_H_*/
#endif
