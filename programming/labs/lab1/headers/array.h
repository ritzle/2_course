#ifndef ARRAY_H
#define ARRAY_H

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
class Array {
 public:
  Array(size_t init_capacity);
  ~Array();

  T& operator[](size_t index);
  void emplace_back(T element);
  void insert(size_t indexInput, T element);
  void erase(size_t indexInput);
  void replace(size_t indexInput, T element);
  int getSize() const;
  void print() const;
  void fillFromFile(const string& filename);

 private:
  T* arr;
  size_t size;
  size_t capacity;
  void resize(size_t new_capacity);
};

#include "../source/array.cpp"

#endif  // ARRAY_H
