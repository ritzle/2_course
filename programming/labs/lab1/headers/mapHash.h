#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <cstddef>
#include <functional>
#include <sstream>  // Добавляем этот заголовок для std::istringstream
#include <stdexcept>

#include "array.h"  // Подключаем вашу реализацию Array
#include "pair.hpp"

// Узел для односвязного списка
template <typename Key, typename Value>
struct Node {
  Pair<Key, Value> data;
  Node* next;

  Node(const Key& key, const Value& value) : data(key, value), next(nullptr) {}
};

template <typename Key, typename Value, unsigned long initial_size>
class Hash_map {
 public:
  Hash_map();   // Конструктор
  ~Hash_map();  // Деструктор

  Value get_value(const Key& key);  // Получение значения по ключу
  void insert(const Key& key, const Value& value);  // Вставка
  void remove(const Key& key);                      // Удаление
  void print();  // Вывод содержимого хеш-таблицы
  void fillFromFile(const std::string& filename);

 private:
  unsigned long hash(const Key& key);  // Хэш-функция
  Array<Node<Key, Value>*>
      table;  // Массив указателей на головы односвязных списков
  unsigned long table_size;  // Текущий размер таблицы
  unsigned long element_count;  // Текущее количество элементов

  void resize(unsigned long new_size);  // Метод увеличения размера
};

#include "../source/hashMap.cpp"  // Включаем реализацию в заголовочный файл

#endif  // HASH_MAP_H
