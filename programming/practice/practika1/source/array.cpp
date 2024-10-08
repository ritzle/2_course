#include "../headers/array.hpp"

#include <iostream>

// Конструктор по умолчанию
template <typename T>
Array<T>::Array() : data(nullptr), capacity(0), size(0) {}

// Изменение размера вектора
template <typename T>
void Array<T>::resize(size_t new_capacity) {
  T* new_data = new T[new_capacity];
  for (size_t i = 0; i < size; ++i) {
    new_data[i] = data[i];
  }
  delete[] data;
  data = new_data;
  capacity = new_capacity;
}

// Добавление элемента в конец
template <typename T>
void Array<T>::push_back(const T& value) {
  if (size == capacity) {
    resize(capacity == 0 ? 1 : capacity * 2);
  }
  data[size] = value;
  ++size;
}

// Оператор доступа по индексу
template <typename T>
T& Array<T>::operator[](size_t index) {
  if (index >= size) {
    throw std::out_of_range("Индекс выходит за пределы вектора");
  }
  return data[index];
}

// Константный оператор доступа по индексу
template <typename T>
const T& Array<T>::operator[](size_t index) const {
  if (index >= size) {
    throw std::out_of_range("Индекс выходит за пределы вектора");
  }
  return data[index];
}

// Получение текущего размера
template <typename T>
size_t Array<T>::getSize() const {
  return size;
}

// Получение вместимости
template <typename T>
size_t Array<T>::getCapacity() const {
  return capacity;
}

// Очистка вектора
template <typename T>
void Array<T>::clear() {
  delete[] data;
  data = nullptr;
  size = 0;
  capacity = 0;
}

// Метод для проверки наличия элемента
template <typename T>
bool Array<T>::contains(const T& value) const {
  for (size_t i = 0; i < size; ++i) {
    if (data[i] == value) {
      return true;
    }
  }
  return false;
}

template <typename T>
void Array<T>::print() const {
  for (size_t i = 0; i < size; ++i) {
    std::cout << data[i] << " ";  // Выводим каждый элемент
  }
  std::cout << std::endl;  // Переход на новую строку после вывода
}