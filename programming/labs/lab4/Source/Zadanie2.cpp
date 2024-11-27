#include "../Headers/Zadanie2.h"


bool isValidTime(const Time& time) 
{
    return (time.hour >= 0 && time.hour < 24) &&
           (time.minute >= 0 && time.minute < 60) &&
           (time.seconds >= 0 && time.seconds < 60);
}


void NotMultithread(vector<Time>& vecTime)
{
    vector <Time> NotTime;

    for (const auto& i : vecTime) 
    {
        if (!isValidTime(i)) 
        {
            NotTime.push_back(i);
        }
    }
}

void processTimes(const vector<Time>& times, vector<Time>& invalidTimes, mutex& mtx) 
{
    vector<Time> localInvalidTimes; 
    for (const auto& i : times) 
    {
        if (!isValidTime(i)) 
        {
            localInvalidTimes.push_back(i);
        }
    }
    
    lock_guard<mutex> lock(mtx);
    invalidTimes.insert(invalidTimes.end(), localInvalidTimes.begin(), localInvalidTimes.end());
}

void WithMultithread(const int& size,const int& numThreads, vector<Time>& times, vector<Time>& invalidTimes)
{
    mutex mutex;

    vector<thread> threads;

    int chunkSize = size / numThreads;

    for (int i = 0; i < numThreads; ++i) 
    {
        int startIdx = i * chunkSize;
        int endIdx = (i == numThreads - 1) ? size : startIdx + chunkSize;
        threads.emplace_back(processTimes, ref(times), ref(invalidTimes), ref(mutex));
    }

    for (auto& thread : threads) 
    {
        thread.join();
    }

}


int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(time(0)));

    if(argc != 3)
    {
        cerr<<"Неверное количество параметров " << argc << " вместо 3"<<endl;
        return -1;
    }

    int quantityThreads = atoi(argv[1]);
    int size = atoi(argv[2]);
    

    vector<Time> invalidTimes;

    vector <Time> vecTime(size);

    int random_number = 0;
   
    for(int i = 0; i < vecTime.size();i++)
    {
        vecTime[i].hour = rand() % 50;
        vecTime[i].minute = rand() % 101;
        vecTime[i].seconds = rand() % 101;
    }

    auto start = chrono::high_resolution_clock::now();

    NotMultithread(vecTime);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> durationWithoutThread = end - start;

    start = chrono::high_resolution_clock::now();

    WithMultithread(size,quantityThreads, vecTime, invalidTimes);

    end = chrono::high_resolution_clock::now();
    chrono::duration<double> durationWithThread = end - start;

    for(auto i : invalidTimes)
    {
        cout<<i.hour<<":"<<i.minute<<":"<<i.seconds<<endl;
    }

    cout<<"Время выполнения без использования многопоточности = " << durationWithoutThread.count() << " миллисекунд ";
    cout<<"Время выполнения c использованием многопоточности = " << durationWithThread.count() << " миллисекунд ";

    return 0;
}