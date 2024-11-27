#include "../Headers/Zadanie1.h"


int main(int argc, char* argv[])
{
    
    if(argc != 4)
    {
        cerr<<"Неверное количество параметров " << argc << " вместо 4"<<endl;
        return -1;
    }

    int quantityThreads = atoi(argv[1]);
    int quantityChars = atoi(argv[2]);
    int numberSyn = atoi(argv[3]);

    Zadanie1 zadanie;

    auto start = chrono::high_resolution_clock::now();

    zadanie.CreateThreads(quantityThreads,quantityChars,numberSyn);
   
    auto end =chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;


    if(numberSyn == 0)
    {
        cout << "\nВремя выполнения в формате race condition: " << duration.count() << " секунд.\n" <<endl;
    }
    else if( numberSyn == 1)
    {
        cout << "\nВремя выполнения с использованием примитива mutex: " << duration.count() << " секунд.\n" <<endl;
    }
    else if( numberSyn == 2)
    {
        cout << "\nВремя выполнения с использованием примитива semaphore: " << duration.count() << " секунд.\n" <<endl;
    }
    else if( numberSyn == 3)
    {
        cout << "\nВремя выполнения с использованием примитива semaphoreSlim: " << duration.count() << " секунд.\n" <<endl;
    }
    else if( numberSyn == 4)
    {
        cout << "\nВремя выполнения с использованием примитива Barrier: " << duration.count() << " секунд.\n" <<endl;
    }
    else if( numberSyn == 5)
    {
        cout << "\nВремя выполнения с использованием примитива SpinLock: " << duration.count() << " секунд.\n" <<endl;
    }
    else if( numberSyn == 6)
    {
        cout << "\nВремя выполнения с использованием примитива SpinWait: " << duration.count() << " секунд.\n" <<endl;
    }
    else if( numberSyn == 7)
    {
        cout << "\nВремя выполнения с использованием примитива Monitor: " << duration.count() << " секунд.\n" <<endl;
    }
    else
    {
        cerr<<"Неизвестный параметр " << numberSyn<<endl;
    }

    return 0;
}