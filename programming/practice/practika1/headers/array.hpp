#ifndef MY_ARR_HPP
#define MY_ARR_HPP

#include <stdexcept>

template <typename T>
class Array {
 private:
  T* data;  // Указатель на динамический массив
  size_t capacity;  // Вместимость контейнера
  size_t size;  // Текущий размер (количество элементов)

  void resize(size_t new_capacity);

 public:
  Array();

  void push_back(const T& value);  // Добавление элемента в конец
  T& operator[](size_t index);  // Оператор доступа по индексу
  const T& operator[](size_t index) const;
  size_t getSize() const;  // Получение текущего размера
  size_t getCapacity() const;  // Получение вместимости
  void clear();                // Очистка вектора

  bool contains(const T& value) const;  // Проверка на наличие элемента

  void print() const;
};

// Реализация функций
#include "../source/array.cpp"

#endif