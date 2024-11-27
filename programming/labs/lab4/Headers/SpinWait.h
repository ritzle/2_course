#ifndef SPINWAIT_H
#define SPINWAIT_H

#include "Headers.h"


class Spinwait
{

    private:
        atomic<bool> spinwait{false};

    public:

    void wait();
    void notify();
    void reset(); 

};

#include "../Source/SpinWait.cpp"


#endif