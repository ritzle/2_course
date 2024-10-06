#include "../headers/array.hpp"

using namespace std;

// Конструктор
template <typename T>
Array<T>::Array(size_t init_capacity)
    : size(0), capacity(init_capacity), arr(new T[init_capacity]) {}

// Деструктор
template <typename T>
Array<T>::~Array() {
  delete[] arr;
}

// Оператор присваивания
template <typename T>
Array<T>& Array<T>::operator=(const Array& other) {
  if (this == &other) {
    return *this;  // Проверка на самоприсваивание
  }

  delete[] arr;  // Освобождаем текущую память

  size = other.size;
  capacity = other.capacity;
  arr = new T[capacity];  // Выделяем новую память

  for (size_t i = 0; i < size; ++i) {
    arr[i] = other.arr[i];  // Копируем элементы
  }

  return *this;
}

// Оператор доступа по индексу
template <typename T>
T& Array<T>::operator[](size_t index) {
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
  T* newArr = new T[new_capacity];
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
    resize(capacity * 2);
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

// Метод заполнения из файла
template <typename T>
void Array<T>::fillFromFile(const string& filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "Could not open the file: " << filename << endl;
    return;
  }

  T value;  // Замените int на T для универсальности
  while (file >> value) {
    emplace_back(value);
  }

  file.close();
}

template <typename T>
T* Array<T>::begin() {
  return arr;
}

template <typename T>
T* Array<T>::end() {
  return arr + size;
}

template <typename T>
const T* Array<T>::begin() const {
  return arr;
}

template <typename T>
const T* Array<T>::end() const {
  return arr + size;
}
