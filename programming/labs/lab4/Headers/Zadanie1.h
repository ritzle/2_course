#ifndef ZADANIE1_H
#define ZADANIE1_H

#include "Headers.h"
#include "semaphoreSlim.h"
#include "SpinLock.h"
#include "SpinWait.h"
#include "Monitor.h"

class Zadanie1
{

    private:

    void GRCRaceCondition(int thread_id, string& buffer,int& quantityChar);
    void GRCMutex(int thread_id, string& buffer, int& quantityChar);
    void GRCSemaphore(int thread_id, string& buffer, int& quantityChar);
    void GRCSemaphoreSlim(int thread_id, string& buffer, int& quantityChar);
    void GRCBarrier(int thread_id, string& buffer, int& quantityChar);
    void GRCSpinLock(int thread_id, string& buffer, int& quantityChar);
    void GRCSpinWait(int thread_id, string& buffer, int& quantityChar);
    void GRCMonitor(int thread_id, string& buffer, int& quantityChar);
    public:

    void CreateThreads(const int& quantityThreads, int& quantityChar,int& numberSyn);






};



#include "../Source/Zadanie1.cpp"


#endif