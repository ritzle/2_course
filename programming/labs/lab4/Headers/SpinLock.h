#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "Headers.h"

class SpinLock
{

private:

    atomic_flag atomic = ATOMIC_FLAG_INIT;
   
public:

    void lock();
    void unlock();

};



#include "../Source/SpinLock.cpp"

#endif