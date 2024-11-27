#include "../Headers/Zadanie3.h"

int main() 
{
    const int num_threads = 5;
    vector<thread> philosophers_threads; 
    vector<mutex> forks(num_threads); 

    // Создаем философов
    for (int i = 0; i < num_threads; ++i) 
    {
        mutex& left_fork = forks[min(i, (i + 1) % num_threads)];
        mutex& right_fork = forks[max(i, (i + 1) % num_threads)];
        philosophers_threads.emplace_back(&Philosopher::action, Philosopher(i, left_fork, right_fork));
    }

   
    for (auto& thread : philosophers_threads) 
    {
        thread.join();
    }

    return 0;
}