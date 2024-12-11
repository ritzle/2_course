#ifndef ZADANIE2_H
#define ZADANIE2_H

#include "Headers.h"


struct Time
{
    int hour;
    int minute;
    int seconds;
};

bool isValidTime(const Time& time);
void WithMultithread(const int& size,const int& numThreads, vector<Time>& times, vector<Time>& invalidTimes);



#endif 