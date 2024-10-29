#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <iostream>
#include <utility>

template <typename T>
class Stack {
 private:
  struct Node {
    T data;
    Node* next;

    Node(T value) : data(value), next(nullptr) {}
  };

  Node* topNode;

 public:
  Stack() : topNode(nullptr) {}
  ~Stack();

  void push(T value);
  void pop();
  T top() const;
  bool isEmpty() const;
};

// Объявление бинарного дерева
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

  void insert(Node*& node, T value);
  bool search(Node* node, T value) const;
  void inorder(Node* node) const;
  void clear(Node* node);

 public:
  BinaryTree();
  ~BinaryTree();

  void insert(T value);
  bool search(T value) const;
  void inorder() const;
  bool isBalanced() const;  // Метод проверки баланса
};

#include "../source/stack.cpp"  // Включаем реализацию

#endif  // BINARYTREE_HPP
