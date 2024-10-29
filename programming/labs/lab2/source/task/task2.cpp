#include <iostream>

#include "../../headers/ATDSet.hpp"

int main() {
  HashSet<int> mySet;  // Создаем множество для целых чисел

  cout << "Вставка элементов:\n";
  cout << "Вставляем 5: " << mySet.insert(5) << endl;
  cout << "Вставляем 10: " << mySet.insert(10) << endl;
  cout << "Вставляем 5 (повторно): " << mySet.insert(5) << endl;

  cout << "\nПроверка наличия элементов:\n";
  cout << "Содержит 5: " << mySet.contains(5) << endl;
  cout << "Содержит 10: " << mySet.contains(10) << endl;
  cout << "Содержит 15: " << mySet.contains(15) << endl;

  cout << "\nУдаление элементов:\n";
  cout << "Удаляем 10: " << mySet.remove(10) << endl;
  cout << "Содержит 10 после удаления: " << mySet.contains(10) << endl;
  cout << "Удаляем 15 (не существует): " << mySet.remove(15) << endl;

  cout << "\nТекущий размер множества: " << mySet.getSize() << endl;

  cout << "\nВставляем 15: " << mySet.insert(15) << endl;
  cout << "Текущий размер множества: " << mySet.getSize() << endl;

  return 0;
}
