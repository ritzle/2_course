#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

using namespace std;

template <typename K, typename V>
struct Pair {
  K key;
  V value;
  bool occupied;

  Pair() : occupied(false) {}  // Initialize occupied as false
};

template <typename K, typename V>
class HashTable {
 private:
  Pair<K, V>* table;
  size_t size;
  size_t capacity;

  void resize(size_t new_capacity);
  size_t getHashIndex(const K& key) const;
  size_t hashString(const string& str) const;
  string convertToString(const K& key) const;

 public:
  HashTable();
  ~HashTable();

  void insert(const K& key, const V& value);
  void delete_by_index(size_t indexInput);
  size_t getSize() const;

  Pair<K, V>& operator[](const K& key);
  Pair<K, V>& operator[](size_t index);

  class Iterator {
   private:
    Pair<K, V>* current;
    size_t capacity;
    Pair<K, V>* table;

   public:
    Iterator(Pair<K, V>* start, size_t cap)
        : current(start), capacity(cap), table(start) {
      // Move iterator to the first occupied element
      while (current < table + cap && !current->occupied) {
        ++current;
      }
    }

    Pair<K, V>& operator*() { return *current; }

    Iterator& operator++() {
      ++current;
      while (current < table + capacity && !current->occupied) {
        ++current;
      }
      return *this;
    }

    bool operator!=(const Iterator& other) const {
      return current != other.current;
    }
  };

  Iterator begin() { return Iterator(table, capacity); }
  Iterator end() { return Iterator(table + capacity, capacity); }
};

#include "../source/hashTable.cpp"

#endif  // HASH_TABLE_H
