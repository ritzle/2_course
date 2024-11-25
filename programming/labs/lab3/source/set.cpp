#include "../headers/set.hpp"

using namespace std;

template <typename T>
Set<T>::Set() : size(0), capacity(10) {  // Начальная вместимость 10
  arr = new T[capacity];  // Выделяем память для массива
}

// Конструктор
template <typename T>
Set<T>::Set(size_t init_capacity) : size(0), capacity(init_capacity) {
  if (init_capacity == 0) {
    throw length_error("Initial capacity must be greater than 0");
  }
  arr = new T[init_capacity]();
}

// Деструктор
template <typename T>
Set<T>::~Set() {
  delete[] arr;
}

// Оператор доступа по индексу
template <typename T>
T& Set<T>::operator[](size_t index) {
  if (index >= size) {
    throw out_of_range("Index out of bounds");
  }
  return arr[index];
}

template <typename T>
void Set<T>::emplace_back(T element) {
  if (contains(element)) {
    return;
  }

  if (size == capacity) {
    resize(capacity * 2);
  }

  arr[size++] = element;
}

// Метод изменения размера массива
template <typename T>
void Set<T>::resize(size_t new_capacity) {
  if (new_capacity < size) {
    throw length_error("New capacity is smaller than current size");
  }
  T* newArr = new T[new_capacity];
  for (size_t i = 0; i < size; i++) {
    newArr[i] = arr[i];
  }
  delete[] arr;
  arr = newArr;
  capacity = new_capacity;
}

// Метод удаления элемента
template <typename T>
void Set<T>::erase(size_t indexInput) {
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
void Set<T>::replace(size_t indexInput, T element) {
  if (indexInput >= size) {
    throw invalid_argument("Index out of bounds");
  }
  arr[indexInput] = element;
}

// Метод получения размера
template <typename T>
int Set<T>::getSize() const {
  return size;
}

// Метод печати массива
template <typename T>
void Set<T>::print() const {
  for (size_t i = 0; i < size; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

template <typename T>
void Set<T>::eraseByValue(const T& value) {
  for (size_t i = 0; i < size; ++i) {
    if (arr[i] == value) {
      erase(i);  // Используем уже существующий метод erase по индексу
      return;  // Удаляем только первый найденный элемент
    }
  }
  std::cerr << "Значение '" << value << "' не найдено в массиве.\n";
}

template <typename T>
bool Set<T>::contains(const T& value) const {
  for (size_t i = 0; i < size; ++i) {
    if (arr[i] == value) {
      return true;
    }
  }
  return false;
}
