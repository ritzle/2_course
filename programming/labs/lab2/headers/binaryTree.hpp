#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <algorithm>
#include <iostream>

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
  int height(Node* node) const;
  bool isBalanced(Node* node) const;

 public:
  BinaryTree();
  ~BinaryTree();

  void insert(T value);
  bool search(T value) const;
  void inorder() const;
  bool isBalanced() const;  // Метод проверки баланса
};

#include "../source/binaryTree.cpp"  // Включаем реализацию

#endif  // BINARYTREE_HPP
