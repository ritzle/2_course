#ifndef MONITOR_H
#define MONITOR_H

#include "Headers.h"

class Monitor 
{
private:
    int shared_resource;
    mutex mtx;
    condition_variable cv;

public:
    Monitor() : shared_resource(0) {}

    void increment() 
    {
        lock_guard<mutex> lock(mtx);
        shared_resource++;
        cv.notify_one(); 
    }

    void wait_for_resource() 
    {
        unique_lock<mutex> lock(mtx);
        while (shared_resource == 0) 
        {
            cv.wait(lock); 
        }
    }
};



#endif