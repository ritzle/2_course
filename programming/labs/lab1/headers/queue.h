#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Queue {
 public:
  Queue();
  ~Queue();

  int GetSize();
  void push(T data);
  void pop();
  void print();

 private:
  struct Node {
    Node* pNext;
    T data;

    Node(T data = T(), Node* pNext = nullptr) : data(data), pNext(pNext) {}
  };

  Node* head;
  int size;
};

#include "../source/queue.cpp"

#endif  // QUEUE_H
