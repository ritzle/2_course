#ifndef SET_H
#define SET_H

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
class Set {
 public:
  Set(size_t init_capacity);
  Set();
  ~Set();

  T& operator[](size_t index);
  void emplace_back(T element);
  void erase(size_t indexInput);
  void eraseByValue(const T& value);

  void replace(size_t indexInput, T element);
  int getSize() const;
  void print() const;
  bool contains(const T& value) const;

 private:
  T* arr;
  size_t size;
  size_t capacity;
  void resize(size_t new_capacity);
};

#include "../source/set.cpp"

#endif  // ARRAY_H
