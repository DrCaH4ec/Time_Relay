#include "inc/time.h"

inline void Time_Inc(struct Time_st * my_time)
{
    my_time->sec++;

    if(my_time->sec > 59){
        my_time->min++;
        my_time->sec = 0;
    }

    if(my_time->min > 59){
        my_time->hour++;
        my_time->min = 0;
    }

    if(my_time->hour > 23)
        my_time->hour = 0;
}



void Time_Dec(struct Time_st * my_time)
{

    if(my_time->sec == 0){
        my_time->sec = 59;

        if(my_time->min == 0){
            my_time->min = 59;

            if(my_time->hour == 0)
                my_time->hour = 23;
            else
                my_time->hour--;
        }
        else{
            my_time->min--;
        }
    }
    else{
        my_time->sec--;
    }
}



void Time_IncMin(struct Time_st * my_time)
{
    uint8_t i;

    for(i = 0; i < 60; i++)
        Time_Inc(my_time);
}



void Time_DecMin(struct Time_st * my_time)
{
    uint8_t i;

    for(i = 0; i < 60; i++)
        Time_Dec(my_time);
}


int8_t Time_Cmp(struct Time_st * my_time1, struct Time_st * my_time2)
{
    if((my_time1->hour - my_time2->hour) == 0){
        if((my_time1->min - my_time2->min) == 0){
            return 0;
        }
        else{
            if((my_time1->min - my_time2->min) > 0)
                return 1;
            else
                return -1;
        }
    }
    else{
        if((my_time1->hour - my_time2->hour) > 0)
            return 1;
        else
            return -1;
    }
}



inline void Time_Reset(struct Time_st * my_time)
{
    my_time->hour = 0;
    my_time->min = 0;
    my_time->sec = 0;
}
