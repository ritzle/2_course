#include "../Headers/semaphoreSlim.h"


SemaphoreSlim::SemaphoreSlim(int count) : count(count), countSem(count) {}


void SemaphoreSlim::release()
{
        countSem.release();
}


void SemaphoreSlim::acquire()
{
        countSem.acquire();
    
}