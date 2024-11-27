#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <algorithm>
#include <fstream>  // Для работы с файлами
#include <iostream>
#include <stdexcept>  // Для исключений

template <typename T>
class BinaryTree {
 private:
  struct Node {
    T data;
    Node* left;
    Node* right;

    Node(T value) : data(value), left(nullptr), right(nullptr) {}
  };

  Node* root;

  // Приватные методы для работы с деревом
  void insert(Node*& node, T value);
  bool search(Node* node, T value) const;
  void inorder(Node* node) const;
  void clear(Node* node);
  int height(Node* node) const;
  bool isBalanced(Node* node) const;
  // Приватные методы для сериализации и десериализации
  void serializeHelper(std::ofstream& ofs,
                       Node* node) const;  // Рекурсивная запись узлов
  Node* deserializeHelper(
      std::ifstream& ifs);  // Рекурсивное восстановление дерева

 public:
  BinaryTree();   // Конструктор
  ~BinaryTree();  // Деструктор

  void insert(T value);        // Вставка элемента
  bool search(T value) const;  // Поиск элемента
  void inorder() const;        // Инордер обход
  bool isBalanced() const;  // Проверка сбалансированности

  // Методы для сериализации и десериализации
  void serialize(const std::string& filename) const;  // Сериализация в файл
  void deserialize(const std::string& filename);  // Десериализация из файла
};

#include "../source/binaryTree.cpp"  // Включаем реализацию

#endif  // BINARYTREE_HPP
