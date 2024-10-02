#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Stack {
 public:
  Stack();
  ~Stack();

  int GetSize();
  void push(T data);
  void pop();
  void print();
  void fillFromFile(const std::string& filename);

 private:
  struct Node {
    Node* pNext;
    T data;

    Node(T data = T(), Node* pNext = nullptr) : data(data), pNext(pNext) {}
  };

  Node* head;
  int size;
};

#include "../source/stack.cpp"

#endif  // STACK_H
