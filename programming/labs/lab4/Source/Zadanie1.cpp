#include "../Headers/Zadanie1.h"



void Zadanie1::GRCRaceCondition(int thread_id, string& buffer, int& quantityChar) 
{
    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;
}

mutex GRCmutex;

void Zadanie1::GRCMutex(int thread_id, string& buffer, int& quantityChar) 
{
    GRCmutex.lock();
    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;

    GRCmutex.unlock();
}

binary_semaphore 
    smphSignalCreateThreadsToGRCSemaphore{0},
    smphSignalGRCSemaphoreToCreateThreads{0};

void Zadanie1::GRCSemaphore(int thread_id, string& buffer, int& quantityChar) 
{   
    smphSignalCreateThreadsToGRCSemaphore.acquire();
    
    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;

    smphSignalGRCSemaphoreToCreateThreads.release();
}


void Zadanie1::GRCSemaphoreSlim(int thread_id, string& buffer, int& quantityChar) 
{   
    SemaphoreSlim smphSlim(2);
    
    smphSlim.release();

    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;

    smphSlim.acquire();
}

barrier barrierGRCBarrier(1);

void Zadanie1::GRCBarrier(int thread_id, string& buffer, int& quantityChar) 
{   
    barrierGRCBarrier.arrive_and_wait();

    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;

}

SpinLock spinlock;

void Zadanie1::GRCSpinLock(int thread_id, string& buffer, int& quantityChar) 
{   
    spinlock.lock();

    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;
    
    spinlock.unlock();

}


Spinwait spinwait;

void Zadanie1::GRCSpinWait(int thread_id, string& buffer, int& quantityChar) 
{   
    

    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;
    
    spinwait.notify();

}

Monitor monitor;

void Zadanie1::GRCMonitor(int thread_id, string& buffer, int& quantityChar) 
{   
    string local_chars;
    
    for (int i = 0; i < quantityChar; ++i) 
    {
        char random_char = static_cast<char>(rand() % (126 - 32 + 1) + 32);
        local_chars += random_char;
    }
    
    buffer += local_chars;
    cout << "Thread " << thread_id << " generated: " << local_chars << endl;
    monitor.wait_for_resource();

}

void Zadanie1::CreateThreads(const int& quantityThreads,int& quantityChar, int& numberSyn)
{
    string buffer;

    srand(static_cast<unsigned int>(time(0))); 

    vector<thread> threads;

    // Создание потоков
    for (int i = 0; i < quantityThreads; ++i) 
    {
        if(numberSyn == 0)
        {
            threads.emplace_back(&Zadanie1::GRCRaceCondition, this, i, ref(buffer), ref(quantityChar));
        }
        else if( numberSyn == 1)
        {
            threads.emplace_back(&Zadanie1::GRCMutex, this, i, ref(buffer), ref(quantityChar));
        }
        else if( numberSyn == 2)
        {
            smphSignalCreateThreadsToGRCSemaphore.release();
            threads.emplace_back(&Zadanie1::GRCSemaphore, this, i, ref(buffer), ref(quantityChar));
            smphSignalGRCSemaphoreToCreateThreads.acquire();
        }
        else if( numberSyn == 3)
        {
            threads.emplace_back(&Zadanie1::GRCSemaphoreSlim, this, i, ref(buffer), ref(quantityChar));
        }
        else if( numberSyn == 4)
        {
            threads.emplace_back(&Zadanie1::GRCBarrier, this, i, ref(buffer), ref(quantityChar));
        }
        else if( numberSyn == 5)
        {
            threads.emplace_back(&Zadanie1::GRCSpinLock, this, i, ref(buffer), ref(quantityChar));
        }
        else if( numberSyn == 6)
        {
            threads.emplace_back(&Zadanie1::GRCSpinWait, this, i, ref(buffer), ref(quantityChar));
            spinwait.wait();
            spinwait.reset();

        }
        else if( numberSyn == 7)
        {
            monitor.increment();
            threads.emplace_back(&Zadanie1::GRCSpinWait, this, i, ref(buffer), ref(quantityChar));
        }
           
        }

    for (auto& thread : threads) 
    {
        thread.join();
    }

    // Вывод общего буфера
   cout << "Buffer: " << buffer << endl;
}

