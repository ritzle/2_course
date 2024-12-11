#ifndef ZADANIE3_H
#define ZADANIE3_H

#include "Headers.h"

class Philosopher 
{
public:

    Philosopher(int id, mutex& _first_fork, mutex& _second_fork);
    void action();
   
    
private:

    int id;
    mutex& left_fork;
    mutex& right_fork;
    int meals_eaten;
    static mutex output_mutex;
};

mutex Philosopher::output_mutex; 

#include "../Source/Zadanie3.cpp"

#endif