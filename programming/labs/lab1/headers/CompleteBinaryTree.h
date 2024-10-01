#ifndef COMPLETEBINARYTREE_H
#define COMPLETEBINARYTREE_H

#include <iostream>
#include <stdexcept>

class CompleteBinaryTree {
 public:
  CompleteBinaryTree(int size);
  void insert(int value);
  bool search(int value);
  bool isComplete();
  void printLevelOrder();
  ~CompleteBinaryTree();

 private:
  struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value);
  };

  struct NodeQueue {
    Node* data;
    NodeQueue* next;
    NodeQueue(Node* value);
  };

  class CustomQueue {
   public:
    CustomQueue();
    ~CustomQueue();
    void enqueue(Node* value);
    void dequeue();
    Node* peek();
    bool isEmpty();
    int getSize();

   private:
    NodeQueue* front;
    NodeQueue* rear;
    int size;
  };

  Node* root;
  int size;
  int currentIndex;

  bool search(Node* node, int value);
  bool isComplete(Node* node, int index, int totalNodes);
  void destroyTree(Node* node);
};

#include "../source/CompleteBinaryTree.cpp"

#endif  // COMPLETEBINARYTREE_H
