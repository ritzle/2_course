#include <iostream>
#include <string>

#include "../../headers/Array.hpp"
#include "../../headers/hashTable.hpp"

using namespace std;

void sortArray(Array<string>& arr);

int main() {
  int N;
  cout << "Введите количество отношений (N): ";
  cin >> N;
  cin.ignore();  // Игнорируем символ новой строки после ввода числа

  // Хэш-таблица для хранения родительских имен и их потомков
  HashTable<string, Array<string>> descendants;

  string child, parent;
  cout << "Введите пары 'ребёнок родитель' (всего " << N - 1 << " пар):\n";
  for (int i = 0; i < N; ++i) {
    cin >> child >> parent;
    // Добавляем потомка к родителю
    descendants[parent].value.emplace_back(child);
  }

  // Сбор имен родителей в массив
  Array<string> names;
  for (auto& pair : descendants) {
    if (pair.occupied) {
      names.emplace_back(pair.key);
    } else {
      continue;
    }
  }

  // Сортируем массив имен родителей
  sortArray(names);

  cout << "\nИмена родителей и количество потомков:\n";
  for (auto& name : names) {
    cout << name << " " << descendants[name].value.getSize() << endl;
  }

  return 0;
}

void quicksort(Array<string>& arr, int left, int right) {
  if (left < right) {
    string pivot =
        arr[right];  // Выбираем последний элемент в качестве опорного
    int i = left - 1;  // Индекс меньшего элемента

    for (int j = left; j < right; j++) {
      if (arr[j] < pivot) {  // Сравниваем с опорным элементом
        i++;
        swap(arr[i], arr[j]);  // Меняем местами
      }
    }
    swap(arr[i + 1],
         arr[right]);  // Перемещаем опорный элемент в правильное место

    // Рекурсивно сортируем подмассивы
    quicksort(arr, left, i);
    quicksort(arr, i + 2, right);
  }
}

// Функция сортировки, которую можно вызывать
void sortArray(Array<string>& arr) { quicksort(arr, 0, arr.getSize() - 1); }
