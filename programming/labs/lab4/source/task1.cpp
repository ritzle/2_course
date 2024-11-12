#include <time.h>

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void getRandomString(int lenString, int numberThread) {
  for (size_t i = 0; i < lenString; i++) {
    int randomNumber = rand() % 26 + 97;

    // this_thread::sleep_for(chrono::milliseconds(rand() % 100));

    cout << "Символ из потока " << numberThread << ": " << char(randomNumber)
         << endl;
  }
}

void startedThread(int counterThread, int lenString) {
  vector<thread> threads;

  for (int i = 1; i <= counterThread; i++) {
    cout << "Запуск потока " << i << endl;
    threads.emplace_back(getRandomString, lenString, i);
  }

  for (auto& th : threads) {
    th.join();
  }
}
int main(int argc, char* argv[]) {
  srand(time(NULL));
  if (argc != 3) {
    cerr << "должно быть 2 аргуменнта" << endl;
    return 0;
  }

  int counterThread = stoi(argv[1]);
  int lenString = stoi(argv[2]);

  startedThread(counterThread, lenString);

  cout << "---------";
}