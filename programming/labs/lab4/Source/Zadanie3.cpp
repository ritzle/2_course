#include "../Headers/Zadanie3.h"

Philosopher::Philosopher(int id, mutex& _first_fork, mutex& _second_fork)
        : id(id), left_fork(_first_fork), right_fork(_second_fork), meals_eaten(0) {}

void Philosopher::action()
{
    while (meals_eaten < 3) 
    { 
        
        {
            lock_guard<mutex> lock(output_mutex);
            cout << "Философ " << id << " думает." << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000 + 1000)); 

        lock(left_fork, right_fork);// захват двух мьютексов сразу для избежания взаимной блокировки
        lock_guard<mutex> lg1(left_fork, adopt_lock); // adopt_lock флаг что мьютекс захвачен
        lock_guard<mutex> lg2(right_fork, adopt_lock); 

       
        {
            lock_guard<mutex> lock(output_mutex);
            cout << "Философ " << id << " начал есть." << endl;
        }
        
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000 + 500)); 
        meals_eaten++; 

        
        {
            lock_guard<mutex> lock(output_mutex);
            cout << "Философ " << id << " освободил вилки." << endl; 
        }

        {
            lock_guard<mutex> lock(output_mutex);
            cout << "\tФилософ " << id << " поел " << meals_eaten << " раз." << endl;
        }
    }

}


