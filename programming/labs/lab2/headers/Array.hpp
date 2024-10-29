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
  Array();
  ~Array();

  T& operator[](size_t index);
  void emplace_back(T element);
  void insert(size_t indexInput, T element);
  void erase(size_t indexInput);
  void eraseByValue(const T& value);
  void replace(size_t indexInput, T element);
  int getSize() const;
  void print() const;
  void writeToFile(const std::string& filename) const;
  void fillFromFile(const std::string& filename);

  // Итератор
  class Iterator {
   public:
    Iterator(T* ptr) : current(ptr) {}

    T& operator*() { return *current; }
    Iterator& operator++() {
      ++current;
      return *this;
    }
    bool operator!=(const Iterator& other) const {
      return current != other.current;
    }

   private:
    T* current;
  };

  // Методы begin и end
  Iterator begin() { return Iterator(arr); }
  Iterator end() { return Iterator(arr + size); }

 private:
  T* arr;
  size_t size;
  size_t capacity;
  void resize(size_t new_capacity);
};

#include "../source/Array.cpp"

#endif  // ARRAY_H
