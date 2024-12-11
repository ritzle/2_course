#ifndef SEMAPHORE_SLIM_H
#define SEMAPHORE_SLIM_H

#include "Headers.h"

class SemaphoreSlim
{

    private:
    
    int count;
    
    counting_semaphore<>  countSem;

    public:

    SemaphoreSlim(int count);

    void release();
    void acquire();

};

#include "../Source/semaphoreSlim.cpp"



#endif