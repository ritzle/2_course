#ifndef MY_MAP_HPP
#define MY_MAP_HPP

#include <stdexcept>  // Для обработки исключений

template <typename K, typename V>
class MyMap {
 private:
  struct Node {
    K key;
    V value;
    Node* next;
    Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
  };
  Node* head;

 public:
  MyMap() : head(nullptr) {}
  ~MyMap();

  // Основные методы
  void insert(const K& key, const V& value);
  V& operator[](const K& key);
  bool contains(const K& key) const;
  void remove(const K& key);

  // Итератор для MyMap
  class Iterator {
   private:
    Node* node;

   public:
    Iterator(Node* n) : node(n) {}

    // Доступ к элементам
    K& getKey() { return node->key; }
    V& getValue() { return node->value; }

    // Операторы для работы с итератором
    Iterator& operator++() {
      if (node) node = node->next;
      return *this;
    }

    bool operator!=(const Iterator& other) const { return node != other.node; }

    // Возвращаем текущий узел
    Node* operator*() { return node; }
  };

  // Методы для получения итераторов
  Iterator begin() { return Iterator(head); }

  Iterator end() { return Iterator(nullptr); }
};

#include "../source/map.cpp"

#endif