#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <cstddef>
#include <functional>  // Для std::hash
#include <stdexcept>

#include "pair.hpp"

template <typename Key, typename Value, unsigned long initial_size>
class Hash_map {
 public:
  Hash_map();
  ~Hash_map();

  Value get_value(const Key& key);
  void insert(const Key& key, const Value& value);
  void remove(const Key& key);

 private:
  unsigned long hash(const Key& key);
  Pair<Key, Value>** table;  // Используем указатель на указатель для
                             // динамического изменения размера
  unsigned long table_size;  // Текущий размер таблицы
  unsigned long element_count;  // Текущее количество элементов

  void resize(unsigned long new_size);
};

#include "../source/hash_map.cpp"  // Включаем реализацию в заголовочный файл

#endif  // HASH_MAP_H
