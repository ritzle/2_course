#include <algorithm>
#include <iostream>
#include <string>

#include "../../headers/Array.hpp"
#include "../../headers/hashTable.hpp"

using namespace std;

// Определяем максимальное количество имен
const int MAX_NAMES = 100;  // Укажите размер в зависимости от ваших данных

HashTable<string, Array<string>> tree;  // имя - массив потомков
HashTable<string, int> descendants_count;  // Хранит количество потомков

// Функция для подсчета потомков
int count_descendants(const string& name) {
  // Если потомков еще не считали
  if (descendants_count[name].valuecd) {  // Проверяем, занята ли ячейка
    int count = 0;

    // Получаем потомков из хэш-таблицы
    for (size_t i = 0; i < tree[name].value.getSize(); ++i) {
      string child = tree[name].value[i];
      count += 1 + count_descendants(child);
    }

    descendants_count.insert(name, count);  // Записываем количество потомков
  }
  return descendants_count[name].value;  // Возвращаем количество потомков
}

int main() {
  int N;
  cin >> N;
  string child, parent;

  for (int i = 0; i < N - 1; ++i) {
    cin >> child >> parent;
    // Добавляем потомка к родителю
    Array<string> children = tree[parent].value;
    children.emplace_back(child);
    tree.insert(parent, children);
  }

  // Подсчет потомков для каждого элемента
  for (const auto& entry : tree) {
    count_descendants(entry.key);
  }

  string names[MAX_NAMES];
  int count = 0;

  // Заполняем массив имен из descendants_count
  for (size_t i = 0; i < descendants_count.getSize(); ++i) {
    names[count++] = descendants_count[i].key;  // Добавляем ключ
  }

  // Сортируем имена
  sort(names, names + count);

  cout << "---------\n";
  // Выводим результат
  for (int i = 0; i < count; ++i) {
    cout << names[i] << " " << descendants_count[names[i]].value
         << endl;  // Выводим имя и количество потомков
  }

  return 0;
}
