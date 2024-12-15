#include "../headers/array.hpp"

// Конструктор по умолчанию
template <typename T>
Array<T>::Array() : head(nullptr), tail(nullptr), size(0) {}

// template <typename T>
// Array<T>::~Array() {}

// Добавление элемента в конец
template <typename T>
void Array<T>::push_back(const T& value) {
  Node* newNode = new Node(value);
  if (tail) {
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
  } else {
    head = tail = newNode;
  }
  ++size;
}

// Вставка элемента в начало
template <typename T>
void Array<T>::insert_beginning(const T& value) {
  Node* newNode = new Node(value);
  if (head) {
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
  } else {
    head = tail = newNode;
  }
  ++size;
}

// Оператор доступа по индексу
template <typename T>
T& Array<T>::operator[](size_t index) {
  if (index >= size) throw std::out_of_range("Index out of range");

  Node* current = head;
  for (size_t i = 0; i < index; ++i) {
    current = current->next;
  }
  return current->data;
}

template <typename T>
const T& Array<T>::operator[](size_t index) const {
  if (index >= size) throw std::out_of_range("Index out of range");

  Node* current = head;
  for (size_t i = 0; i < index; ++i) {
    current = current->next;
  }
  return current->data;
}

// Получение текущего размера
template <typename T>
size_t Array<T>::getSize() const {
  return size;
}

// Очистка списка
template <typename T>
void Array<T>::clear() {
  while (head) {
    Node* temp = head;
    head = head->next;
    delete temp;
  }
  head = tail = nullptr;
  size = 0;
}

// Проверка на пустоту
template <typename T>
bool Array<T>::empty() const {
  return size == 0;
}

// Возврат последнего элемента
template <typename T>
T& Array<T>::back() {
  if (!tail) throw std::out_of_range("List is empty");
  return tail->data;
}

// Возврат первого элемента
template <typename T>
T& Array<T>::front() {
  if (!head) throw std::out_of_range("List is empty");
  return head->data;
}

// Удаление элемента по индексу
template <typename T>
void Array<T>::erase(size_t index) {
  if (index >= size) throw std::out_of_range("Index out of range");

  Node* current = head;
  for (size_t i = 0; i < index; ++i) {
    current = current->next;
  }

  if (current->prev)
    current->prev->next = current->next;
  else
    head = current->next;

  if (current->next)
    current->next->prev = current->prev;
  else
    tail = current->prev;

  delete current;
  --size;
}

// Проверка на наличие элемента
template <typename T>
bool Array<T>::contains(const T& value) const {
  Node* current = head;
  while (current) {
    if (current->data == value) return true;
    current = current->next;
  }
  return false;
}

// Вывод элементов списка
template <typename T>
void Array<T>::print() const {
  Node* current = head;
  while (current) {
    std::cout << current->data << " ";
    current = current->next;
  }
  std::cout << std::endl;
}

// Метод для создания копии списка
template <typename T>
Array<T> Array<T>::copy() const {
  Array<T> newArray;  // Новый объект Array для копии
  Node* current = head;
  while (current) {
    newArray.push_back(
        current->data);  // Добавляем каждый элемент в новый список
    current = current->next;
  }
  return newArray;
}

// просто через пробел
template <typename T>
std::string Array<T>::to_stringJson() const {
  std::stringstream ss;

  Node* current = head;
  while (current) {
    ss << current->data;
    current = current->next;
    if (current) {
      ss << " ";
    }
  }

  return ss.str();  // Возвращаем строку в формате JSON
}

template <typename T>
std::string Array<T>::to_stringComand() const {
  std::stringstream ss;

  ss << "(";  // Начало строки

  Node* current = head;
  while (current) {
    ss << "'" << current->data << "'";  // Добавляем кавычки вокруг данных
    current = current->next;
    if (current) {
      ss << ", ";  // Разделитель для элементов
    }
  }

  ss << ")";  // Конец строки

  return ss.str();  // Возвращаем строку в виде команды
}

template <typename T>
void Array<T>::from_stringJson(const std::string& input) {
  std::istringstream stream(
      input);  // Используем строковый поток для разбора строки
  T value;

  clear();

  while (stream >> value) {
    push_back(value);
  }
}