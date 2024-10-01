#include "../headers/array.h"

#include <stdexcept>

using namespace std;

// Конструктор
template <typename T>
Array<T>::Array(size_t init_capacity) : size(0), capacity(init_capacity) {
  if (init_capacity == 0) {
    throw length_error("Initial capacity must be greater than 0");
  }
  arr = new T[init_capacity]();
}

// Деструктор
template <typename T>
Array<T>::~Array() {
  delete[] arr;
}

// Оператор доступа по индексу
template <typename T>
T &Array<T>::operator[](size_t index) {
  if (index >= size) {
    throw out_of_range("Index out of bounds");
  }
  return arr[index];
}

// Метод добавления элемента
template <typename T>
void Array<T>::emplace_back(T element) {
  if (size == capacity) {
    resize(capacity * 2);
  }
  arr[size++] = element;
}

// Метод изменения размера массива
template <typename T>
void Array<T>::resize(size_t new_capacity) {
  if (new_capacity < size) {
    throw length_error("New capacity is smaller than current size");
  }
  T *newArr = new T[new_capacity];
  for (size_t i = 0; i < size; i++) {
    newArr[i] = arr[i];
  }
  delete[] arr;
  arr = newArr;
  capacity = new_capacity;
}

// Метод вставки элемента
template <typename T>
void Array<T>::insert(size_t indexInput, T element) {
  if (indexInput > size) {
    throw invalid_argument("Index out of bounds");
  }

  if (size == capacity) {
    resize(capacity * 2);  // Увеличиваем размер массива
  }

  for (size_t i = size; i > indexInput; --i) {
    arr[i] = arr[i - 1];
  }
  arr[indexInput] = element;
  size++;
}

// Метод удаления элемента
template <typename T>
void Array<T>::erase(size_t indexInput) {
  if (indexInput >= size) {
    throw invalid_argument("Index out of bounds");
  }

  for (size_t i = indexInput; i < size - 1; i++) {
    arr[i] = arr[i + 1];
  }
  size--;
}

// Метод замены элемента
template <typename T>
void Array<T>::replace(size_t indexInput, T element) {
  if (indexInput >= size) {
    throw invalid_argument("Index out of bounds");
  }
  arr[indexInput] = element;
}

// Метод получения размера
template <typename T>
int Array<T>::getSize() const {
  return size;
}

// Метод печати массива
template <typename T>
void Array<T>::print() const {
  for (size_t i = 0; i < size; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

// Экспортируем реализацию для конкретного типа, например, int
// template class Array<int>;
