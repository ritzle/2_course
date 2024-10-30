#include <iostream>

#include "../../headers/treeStack.hpp"

int main() {
  BinaryTreeStack<int> tree;

  // Вставляем элементы в дерево
  tree.insert(40);
  tree.insert(20);
  tree.insert(60);
  tree.insert(10);
  tree.insert(30);
  tree.insert(50);
  tree.insert(70);

  std::cout << "Итеративный симметричный обход бинарного дерева:\n";
  std::cout << "-----------------------------------------------\n";
  std::cout << "Элементы дерева в порядке обхода: ";

  // Используем метод итеративного обхода через стек
  tree.inorderIterative();

  std::cout << "\n-----------------------------------------------\n";

  return 0;
}
