#ifndef COMPLETEBINARYTREE_H
#define COMPLETEBINARYTREE_H

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class CompleteBinaryTree {
 public:
  CompleteBinaryTree(int size);
  void insert(int value);
  bool search(int value);
  bool isComplete();
  void printLevelOrder();
  void fillTreeFromFile(const std::string& filename);  // New method for filling
                                                       // the tree from a file
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
    void fillFromFile(
        const std::string& filename);  // Correct usage of std::string

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
