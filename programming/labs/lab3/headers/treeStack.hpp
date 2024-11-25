#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <climits>
#include <iostream>
#include <utility>

template <typename T>
class BinaryTreeStack {
 private:
  struct Node {
    T data;
    Node* left;
    Node* right;
    Node(T value) : data(value), left(nullptr), right(nullptr) {}
  };

  Node* root;

  void insert(Node*& node, T value);
  bool search(Node* node, T value) const;
  void clear(Node* node);

 public:
  BinaryTreeStack();
  ~BinaryTreeStack();

  void insert(T value);
  bool search(T value) const;
  void inorderIterative()
      const;  // Метод симметричного обхода с использованием стека
  bool isBalanced() const;  // Метод проверки баланса дерева
};

// Определение стека (внутри того же файла для использования в обходе дерева)
template <typename T>
class Stack {
 private:
  struct StackNode {
    T data;
    StackNode* next;
    StackNode(T value) : data(value), next(nullptr) {}
  };

  StackNode* topNode;

 public:
  Stack() : topNode(nullptr) {}
  ~Stack();

  void push(T value);
  void pop();
  T top() const;
  bool isEmpty() const;
};

#include "../source/treeStack.cpp"  // Включаем файл реализации

#endif  // BINARYTREE_HPP
