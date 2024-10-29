#include <iostream>

#include "../../headers/binaryTree.hpp"

int main() {
  BinaryTree<int> tree;
  int value;

  std::cout << "Введите последовательность целых чисел, заканчивающуюся нулем:"
            << std::endl;

  while (true) {
    std::cin >> value;
    if (value == 0) break;  // Завершаем ввод, если число 0
    tree.insert(value);
  }

  std::cout << (tree.isBalanced() ? "YES" : "NO") << std::endl;

  return 0;
}
