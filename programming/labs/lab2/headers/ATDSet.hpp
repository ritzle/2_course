#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

using namespace std;

template <typename K>
class HashSet {
 private:
  struct Pair {
    K key;
    bool occupied;  // Флаг, указывающий, занята ли ячейка
  };

  Pair* table;      // Таблица пар
  size_t size;      // Текущий размер множества
  size_t capacity;  // Вместимость множества

  string convertToString(const K& key) const;  // Конвертация ключа в строку
  size_t hashString(const string& str) const;  // Хеширование строки
  size_t getHashIndex(const K& key) const;  // Получение индекса хеша
  void resize(size_t new_capacity);  // Изменение размера таблицы

 public:
  HashSet();                          // Конструктор
  ~HashSet();                         // Деструктор
  bool insert(const K& key);          // Вставка ключа
  bool contains(const K& key) const;  // Проверка на наличие ключа
  bool remove(const K& key);          // Удаление ключа
  size_t getSize() const;  // Получение текущего размера множества

  void print() const;
};

#include "../source/ATDSet.cpp"
#endif  // HASHSET_HPP
