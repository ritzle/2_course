#include <iostream>

#include "../../headers/set.hpp"
template <typename T>
Set<T> Union(Set<T>& set1, Set<T>& set2);

template <typename T>
Set<T> Intersection(Set<T>& set1, Set<T>& set2);

template <typename T>
Set<T> Difference(Set<T>& set1, Set<T>& set2);

int main() {
  char operation;
  while (true) {
    // Запрос ввода двух множеств
    int size1, size2;
    std::cout << "Enter size of first set: ";
    std::cin >> size1;
    Set<int> set1(size1);

    std::cout << "Enter elements of first set:\n";
    for (int i = 0; i < size1; ++i) {
      int element;
      std::cin >> element;
      set1.emplace_back(element);
    }

    std::cout << "Enter size of second set: ";
    std::cin >> size2;
    Set<int> set2(size2);

    std::cout << "Enter elements of second set:\n";
    for (int i = 0; i < size2; ++i) {
      int element;
      std::cin >> element;
      set2.emplace_back(element);
    }

    // Запрос операции
    std::cout << "Enter operation (U - Union, I - Intersection, D - "
                 "Difference, Q - Quit): ";
    std::cin >> operation;

    switch (operation) {
      case 'U':
      case 'u': {
        Set<int> result = Union(set1, set2);
        std::cout << "Union result:\n";
        result.print();  // Предполагается, что у вас есть метод print в классе
                         // Set
        break;
      }
      case 'I':
      case 'i': {
        Set<int> result = Intersection(set1, set2);
        std::cout << "Intersection result:\n";
        result.print();  // Предполагается, что у вас есть метод print в классе
                         // Set
        break;
      }
      case 'D':
      case 'd': {
        Set<int> result = Difference(set1, set2);
        std::cout << "Difference result:\n";
        result.print();  // Предполагается, что у вас есть метод print в классе
                         // Set
        break;
      }
      case 'Q':
      case 'q':
        std::cout << "Exiting program.\n";
        return 0;  // Выход из программы
      default:
        std::cout << "Invalid operation. Please try again.\n";
        break;
    }
  }
  return 0;
}

template <typename T>
Set<T> Union(Set<T>& set1, Set<T>& set2) {
  Set<T> result(
      set1.getSize() +
      set2.getSize());  // Инициализация результата с достаточной емкостью

  for (int i = 0; i < set1.getSize(); ++i) {
    result.emplace_back(set1[i]);
  }

  for (int i = 0; i < set2.getSize(); ++i) {
    result.emplace_back(set2[i]);
  }

  return result;
}

template <typename T>
Set<T> Intersection(Set<T>& set1, Set<T>& set2) {
  Set<T> result(set1.getSize() < set2.getSize() ? set1.getSize()
                                                : set2.getSize());

  for (int i = 0; i < set1.getSize(); ++i) {
    if (set2.contains(set1[i])) {
      result.emplace_back(set1[i]);
    }
  }

  return result;
}

template <typename T>
Set<T> Difference(Set<T>& set1, Set<T>& set2) {
  Set<T> result(set1.getSize());

  for (int i = 0; i < set1.getSize(); ++i) {
    if (!set2.contains(set1[i])) {
      result.emplace_back(set1[i]);
    }
  }

  return result;
}
