#include "../headers/hashTable.hpp"

template <typename K, typename V>
string HashTable<K, V>::convertToString(const K& key) const {
  if constexpr (is_arithmetic<K>::value) {
    ostringstream oss;
    oss << key;
    return oss.str();
  } else {
    return string(key);
  }
}

template <typename K, typename V>
size_t HashTable<K, V>::hashString(const string& str) const {
  size_t hash = 3;
  for (char c : str) {
    hash = hash * 13 + c;
  }
  return hash;
}

template <typename K, typename V>
size_t HashTable<K, V>::getHashIndex(const K& key) const {
  string keyStr = convertToString(key);
  return hashString(keyStr) % capacity;
}

template <typename K, typename V>
HashTable<K, V>::HashTable() : size(0), capacity(10) {
  table = new Pair<K, V>[capacity]();  // Zero-initialize the array
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
  delete[] table;
}

template <typename K, typename V>
Pair<K, V>& HashTable<K, V>::operator[](const K& key) {
  size_t index = getHashIndex(key);

  // If the pair is occupied and the key matches, return it
  if (table[index].occupied && table[index].key == key) {
    return table[index];
  }

  // Linear probing for an empty spot
  while (table[index].occupied) {
    index = (index + 1) % capacity;
  }

  // Create a new pair
  table[index].key = key;
  table[index].occupied = true;
  size++;

  return table[index];
}

template <typename K, typename V>
void HashTable<K, V>::resize(size_t new_capacity) {
  Pair<K, V>* newTable = new Pair<K, V>[new_capacity]();

  // Rehash existing entries
  for (size_t i = 0; i < capacity; i++) {
    if (table[i].occupied) {
      size_t index = getHashIndex(table[i].key) % new_capacity;
      while (newTable[index].occupied) {
        index = (index + 1) % new_capacity;  // Linear probing
      }
      newTable[index] = table[i];
    }
  }

  delete[] table;
  table = newTable;
  capacity = new_capacity;
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
  if (size == capacity) {
    resize(capacity * 2);
  }

  Pair<K, V>& pair = (*this)[key];  // Use the operator[] to insert
  pair.value = value;               // Update value if the key already exists
}

template <typename K, typename V>
size_t HashTable<K, V>::getSize() const {
  return size;  // Return the current size (number of occupied entries)
}

template <typename K, typename V>
Pair<K, V>& HashTable<K, V>::operator[](size_t index) {
  if (index >= capacity) {
    cerr << "Index out of bounds" << endl;
    throw out_of_range("Index out of bounds");
  }
  return table[index];
}

template <typename K, typename V>
void HashTable<K, V>::delete_by_index(size_t indexInput) {
  if (indexInput >= capacity || !table[indexInput].occupied) {
    cerr << "Invalid index" << endl;
    return;
  }
  table[indexInput].occupied = false;  // Mark as empty
  size--;

  // Rehashing may be required to maintain probing
  for (size_t i = (indexInput + 1) % capacity; table[i].occupied;
       i = (i + 1) % capacity) {
    Pair<K, V> temp = table[i];  // Store the element to rehash
    table[i].occupied = false;   // Mark as empty
    size--;
    (*this)[temp.key] = temp.value;  // Reinsert the element
  }
}
