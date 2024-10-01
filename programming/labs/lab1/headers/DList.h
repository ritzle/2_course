#ifndef DLIST_H
#define DLIST_H

#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class DList {
 public:
  DList();
  ~DList();

  int GetSize();
  void push_back(T data);
  void push_front(T data);
  void pop_front();
  void pop_back();
  void pop_value(T searchData);
  void search_by_value(T searchData);
  T& operator[](size_t index);
  void print();

 private:
  struct Node {
    Node* pNext;
    Node* pPrev;
    T data;

    Node(T data = T(), Node* pNext = nullptr, Node* pPrev = nullptr)
        : data(data), pNext(pNext), pPrev(pPrev) {}
  };

  Node* head;
  Node* tail;
  int size;
};

#include "../source/DList.cpp"

#endif  // DLIST_H
