#include "../Headers/SpinLock.h"


void SpinLock::lock()
{
    while (atomic.test_and_set(memory_order_acquire)) 
    {
            
    }
}

void SpinLock::unlock()
{
    atomic.clear(memory_order_release);
}