#ifndef OLIST_H
#define OLIST_H

#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class OList {
 public:
  OList();
  ~OList();

  int GetSize();
  void push_back(T data);
  void push_front(T data);
  void pop_front();
  void pop_back();
  void pop_value(T searchData);
  void search_by_value(T searchData);
  T& operator[](size_t index);
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

#include "../source/OList.cpp"

#endif  // OLIST_H
