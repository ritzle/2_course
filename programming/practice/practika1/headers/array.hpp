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
  void insert_beginning(const T& value);
  T& operator[](size_t index);  // Оператор доступа по индексу
  const T& operator[](size_t index) const;
  size_t getSize() const;  // Получение текущего размера
  size_t getCapacity() const;  // Получение вместимости
  void clear();                // Очистка вектора
  bool empty();
  T& back();

  bool contains(const T& value) const;  // Проверка на наличие элемента

  void print() const;

  // Итераторы
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
    bool operator==(const Iterator& other) const {
      return current == other.current;
    }

   private:
    T* current;  // Указатель на текущий элемент
  };

  Iterator begin() { return Iterator(data); }  // Начало итератора
  Iterator end() { return Iterator(data + size); }

  // Итераторы для константных объектов
  class ConstIterator {
   public:
    ConstIterator(const T* ptr) : current(ptr) {}

    const T& operator*() const { return *current; }
    ConstIterator& operator++() {
      ++current;
      return *this;
    }
    bool operator!=(const ConstIterator& other) const {
      return current != other.current;
    }
    bool operator==(const ConstIterator& other) const {
      return current == other.current;
    }

   private:
    const T* current;  // Указатель на текущий элемент
  };

  ConstIterator begin() const { return ConstIterator(data); }
  ConstIterator end() const { return ConstIterator(data + size); }
};

// Реализация функций
#include "../source/array.cpp"

#endif