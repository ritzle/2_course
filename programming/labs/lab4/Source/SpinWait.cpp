#include "../Headers/SpinWait.h"


void Spinwait:: wait() 
{
    while (spinwait.load(memory_order_acquire) == false) 
    {
        
    }
}

void Spinwait::notify() 
{
    
    spinwait.store(true, memory_order_release);
}

void Spinwait::reset() 
{
    
    spinwait.store(false, memory_order_release);
}