#include "../headers/hash_map.hpp"

#include "../headers/Table.hpp"

// Конструктор
template <typename Key, typename Value, unsigned long initial_size>
Hash_map<Key, Value, initial_size>::Hash_map() {
  table_size = initial_size;
  element_count = 0;  // Инициализация счётчика элементов
  table = new Pair<Key, Value>*[table_size];
  for (unsigned long i = 0; i < table_size; i++) {
    table[i] = nullptr;
  }
}

// Деструктор
template <typename Key, typename Value, unsigned long initial_size>
Hash_map<Key, Value, initial_size>::~Hash_map() {
  for (unsigned long i = 0; i < table_size; i++) {
    Pair<Key, Value>* entry = table[i];
    while (entry != nullptr) {
      Pair<Key, Value>* temp = entry;
      entry = entry->next;
      delete temp;  // Освобождение памяти для каждого элемента
    }
  }
  delete[] table;  // Освобождаем память для таблицы
}

// Хеш-функция
template <typename Key, typename Value, unsigned long initial_size>
unsigned long Hash_map<Key, Value, initial_size>::hash(const Key& key) {
  return std::hash<Key>()(key) % table_size;  // Хеш-функция
}

// Получение значения по ключу
template <typename Key, typename Value, unsigned long initial_size>
Value Hash_map<Key, Value, initial_size>::get_value(const Key& key) {
  unsigned long hash_value = hash(key);
  Pair<Key, Value>* entry = table[hash_value];

  while (entry != nullptr) {
    if (entry->key == key) {
      return entry->value;
    }
    entry = entry->next;
  }

  throw std::runtime_error("Ключ не найден");
}

// Вставка ключа-значения
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::insert(const Key& key,
                                                const Value& value) {
  if (element_count >= static_cast<unsigned long>(table_size * 0.7)) {
    resize(table_size * 2);  // Увеличиваем размер, если заполнено более 70%
  }

  unsigned long hash_value = hash(key);
  Pair<Key, Value>* current = table[hash_value];

  // Проверка на наличие существующего ключа
  while (current != nullptr) {
    if (current->key == key) {
      current->value = value;  // Обновляем значение, если ключ уже существует
      return;
    }
    current = current->next;
  }

  // Если ключ не найден, добавляем новый элемент
  Pair<Key, Value>* new_entry = new Pair<Key, Value>(key, value);
  new_entry->next = table[hash_value];  // Добавляем в начало списка
  table[hash_value] = new_entry;
  element_count++;  // Увеличиваем счётчик элементов
}

// Удаление по ключу
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::remove(const Key& key) {
  unsigned long hash_value = hash(key);
  Pair<Key, Value>* current = table[hash_value];
  Pair<Key, Value>* previous = nullptr;

  while (current != nullptr) {
    if (current->key == key) {
      if (previous == nullptr) {
        table[hash_value] = current->next;  // Удаляем первый элемент
      } else {
        previous->next = current->next;  // Удаляем элемент, не первый
      }
      delete current;
      element_count--;  // Уменьшаем счётчик элементов
      return;
    }
    previous = current;
    current = current->next;
  }

  throw std::runtime_error("Ключ не найден для удаления");
}

// Функция изменения размера хэш-таблицы
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::resize(unsigned long new_size) {
  Pair<Key, Value>** new_table = new Pair<Key, Value>*[new_size];
  for (unsigned long i = 0; i < new_size; i++) {
    new_table[i] = nullptr;
  }

  // Перераспределяем существующие пары в новую таблицу
  for (unsigned long i = 0; i < table_size; i++) {
    Pair<Key, Value>* entry = table[i];
    while (entry != nullptr) {
      unsigned long new_hash_value = std::hash<Key>()(entry->key) % new_size;
      Pair<Key, Value>* temp = entry;
      entry = entry->next;

      // Вставляем в новую таблицу
      temp->next = new_table[new_hash_value];
      new_table[new_hash_value] = temp;
    }
  }

  // Удаляем старую таблицу
  delete[] table;

  // Обновляем указатели на новую таблицу и размер
  table = new_table;
  table_size = new_size;
}

template class Hash_map<std::string, Table, 10>;
