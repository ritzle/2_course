#include "../headers/map.hpp"

// Деструктор для удаления всех узлов
template <typename K, typename V>
MyMap<K, V>::~MyMap() {
  Node* current = head;
  while (current) {
    Node* next = current->next;
    delete current;
    current = next;
  }
}

// Вставка новой пары ключ-значение
template <typename K, typename V>
void MyMap<K, V>::insert(const K& key, const V& value) {
  if (contains(key)) {
    throw std::runtime_error("Ключ уже существует");
  }
  Node* newNode = new Node(key, value);
  newNode->next = head;
  head = newNode;
}

// Доступ по ключу
template <typename K, typename V>
V& MyMap<K, V>::operator[](const K& key) {
  Node* current = head;
  while (current) {
    if (current->key == key) {
      return current->value;
    }
    current = current->next;
  }
  insert(key,
         V());  // Если ключ не найден, добавляем его с значением по умолчанию
  return head->value;
}

// Проверка наличия ключа
template <typename K, typename V>
bool MyMap<K, V>::contains(const K& key) const {
  Node* current = head;
  while (current) {
    if (current->key == key) {
      return true;
    }
    current = current->next;
  }
  return false;
}

// Удаление пары по ключу
template <typename K, typename V>
void MyMap<K, V>::remove(const K& key) {
  Node** current = &head;
  while (*current) {
    if ((*current)->key == key) {
      Node* temp = *current;
      *current = (*current)->next;
      delete temp;
      return;
    }
    current = &((*current)->next);
  }
}
