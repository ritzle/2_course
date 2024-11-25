#include "../headers/ATDSet.hpp"

template <typename K>
string HashSet<K>::convertToString(const K& key) const {
  if constexpr (is_arithmetic<K>::value) {
    ostringstream oss;
    oss << key;
    return oss.str();
  } else {
    return string(key);
  }
}

template <typename K>
size_t HashSet<K>::hashString(const string& str) const {
  size_t hash = 3;
  for (char c : str) {
    hash = hash * 13 + c;
  }
  return hash;
}

template <typename K>
size_t HashSet<K>::getHashIndex(const K& key) const {
  string keyStr = convertToString(key);
  return hashString(keyStr) % capacity;
}

template <typename K>
HashSet<K>::HashSet() : size(0), capacity(10) {
  table = new Pair[capacity]();  // Инициализация массива пар
}

template <typename K>
HashSet<K>::~HashSet() {
  delete[] table;  // Освобождение памяти
}

template <typename K>
bool HashSet<K>::insert(const K& key) {
  if (contains(key)) {
    return false;  // Ключ уже существует
  }
  if (size == capacity) {
    resize(capacity * 2);  // Увеличение размера при необходимости
  }

  size_t index = getHashIndex(key);

  // Линейный поиск свободной ячейки
  while (table[index].occupied) {
    index = (index + 1) % capacity;
  }

  // Вставка нового ключа
  table[index].key = key;
  table[index].occupied = true;
  size++;
  return true;
}

template <typename K>
bool HashSet<K>::contains(const K& key) const {
  size_t index = getHashIndex(key);

  // Линейный поиск ключа
  while (table[index].occupied) {
    if (table[index].key == key) {
      return true;  // Ключ найден
    }
    index = (index + 1) % capacity;
  }
  return false;  // Ключ не найден
}

template <typename K>
bool HashSet<K>::remove(const K& key) {
  size_t index = getHashIndex(key);

  // Линейный поиск ключа
  while (table[index].occupied) {
    if (table[index].key == key) {
      table[index].occupied = false;  // Пометить как удаленный
      size--;
      // Возможно, потребуется повторное хеширование
      for (size_t i = (index + 1) % capacity; table[i].occupied;
           i = (i + 1) % capacity) {
        K tempKey = table[i].key;  // Сохранить ключ для повторного хеширования
        table[i].occupied = false;  // Пометить как пустую
        size--;
        insert(tempKey);  // Повторно вставить элемент
      }
      return true;  // Ключ удален
    }
    index = (index + 1) % capacity;
  }
  return false;  // Ключ не найден
}

template <typename K>
size_t HashSet<K>::getSize() const {
  return size;  // Возврат текущего размера множества
}

template <typename K>
void HashSet<K>::resize(size_t new_capacity) {
  Pair* newTable = new Pair[new_capacity]();
  size_t oldSize = size;

  // Rehash existing elements
  for (size_t i = 0; i < capacity; ++i) {
    if (table[i].occupied) {
      size_t newIndex = hashString(std::to_string(table[i].key)) % new_capacity;
      while (newTable[newIndex].occupied) {
        newIndex = (newIndex + 1) % new_capacity;  // Linear probing
      }
      newTable[newIndex] = table[i];
    }
  }

  delete[] table;
  table = newTable;
  capacity = new_capacity;
}

template <typename K>
void HashSet<K>::print() const {
  std::cout << "Элементы множества:\n";
  for (size_t i = 0; i < capacity; ++i) {
    if (table[i].occupied) {
      std::cout << table[i].key << " ";
    }
  }
  std::cout << std::endl;
}