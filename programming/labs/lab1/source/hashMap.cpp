#include <iostream>

#include "../headers/mapHash.h"

template <typename Key, typename Value, unsigned long initial_size>
Hash_map<Key, Value, initial_size>::Hash_map()
    : table(initial_size), table_size(initial_size), element_count(0) {
  // Инициализируем все элементы массива `table` как nullptr
  for (unsigned long i = 0; i < table_size; ++i) {
    table[i] = nullptr;  // Каждый бакет указывает на null
  }
}

// Деструктор
template <typename Key, typename Value, unsigned long initial_size>
Hash_map<Key, Value, initial_size>::~Hash_map() {
  for (unsigned long i = 0; i < table_size; ++i) {
    Node<Key, Value>* current = table[i];
    while (current) {
      Node<Key, Value>* temp = current;
      current = current->next;
      delete temp;  // Освобождаем память
    }
  }
}

// Хэш-функция
template <typename Key, typename Value, unsigned long initial_size>
unsigned long Hash_map<Key, Value, initial_size>::hash(const Key& key) {
  return std::hash<Key>()(key) %
         table_size;  // Хэширование с использованием std::hash
}

// Метод вставки
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::insert(const Key& key,
                                                const Value& value) {
  unsigned long index = hash(key);
  Node<Key, Value>* newNode = new Node<Key, Value>(key, value);

  // Проверка на существование ключа
  Node<Key, Value>* current = table[index];
  while (current) {
    if (current->data.key == key) {
      current->data.value = value;  // Обновляем значение
      delete newNode;  // Удаляем временный узел
      return;
    }
    current = current->next;
  }

  // Добавляем новый узел в начало списка
  newNode->next = table[index];
  table[index] = newNode;
  element_count++;

  // Увеличиваем размер, если все бакеты заполнены
  if (element_count > table_size) {
    resize(table_size * 2);
  }
}

// Метод получения значения по ключу
template <typename Key, typename Value, unsigned long initial_size>
Value Hash_map<Key, Value, initial_size>::get_value(const Key& key) {
  unsigned long index = hash(key);
  Node<Key, Value>* current = table[index];

  while (current) {
    if (current->data.first == key) {
      return current->data.second;  // Возвращаем значение
    }
    current = current->next;
  }

  throw std::runtime_error("Key not found");  // Если ключ не найден
}

// Метод удаления
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::remove(const Key& key) {
  unsigned long index = hash(key);
  Node<Key, Value>* current = table[index];
  Node<Key, Value>* previous = nullptr;

  while (current) {
    if (current->data.first == key) {
      if (previous) {
        previous->next = current->next;  // Удаляем узел из списка
      } else {
        table[index] = current->next;  // Обновляем голову списка
      }
      delete current;  // Освобождаем память
      element_count--;
      return;
    }
    previous = current;
    current = current->next;
  }
}

// Метод увеличения размера
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::resize(unsigned long new_size) {
  Array<Node<Key, Value>*> newTable(new_size);

  for (unsigned long i = 0; i < new_size; ++i) {
    newTable[i] = nullptr;  // Инициализируем новый массив
  }

  // Переносим элементы из старого массива в новый
  for (unsigned long i = 0; i < table_size; ++i) {
    Node<Key, Value>* current = table[i];
    while (current) {
      Node<Key, Value>* next = current->next;
      unsigned long newIndex = std::hash<Key>()(current->data.key) % new_size;

      // Добавляем в новый массив
      current->next = newTable[newIndex];
      newTable[newIndex] = current;

      current = next;  // Переход к следующему узлу
    }
  }

  // Обновляем таблицу и размер
  table = newTable;
  table_size = new_size;
}

// Метод для вывода содержимого хеш-таблицы
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::print() {
  for (unsigned long i = 0; i < table_size; ++i) {
    Node<Key, Value>* current = table[i];
    if (current) {
      std::cout << "Bucket " << i << ": ";
      while (current) {
        std::cout << "[" << current->data.key << ": " << current->data.value
                  << "] -> ";
        current = current->next;
      }
      std::cout << "nullptr" << std::endl;
    }
  }
}

// Метод для заполнения хеш-таблицы из файла
template <typename Key, typename Value, unsigned long initial_size>
void Hash_map<Key, Value, initial_size>::fillFromFile(
    const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open the file: " << filename << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    Key key;
    Value value;

    // Предполагается, что в файле ключ и значение разделены пробелом
    if (iss >> key >> value) {
      insert(key, value);  // Вставляем пару в хеш-таблицу
    }
  }

  file.close();
}