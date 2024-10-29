#include "../headers/Array.hpp"

using namespace std;

template <typename T>
Array<T>::Array() : size(0), capacity(10) {  // Начальная вместимость 10
  arr = new T[capacity];  // Выделяем память для массива
}

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

template <typename T>
void Array<T>::writeToFile(const std::string& filename) const {
  std::ofstream file(filename);  // Открываем файл для записи
  if (!file.is_open()) {
    std::cerr << "Failed to open file for writing!" << std::endl;
    return;
  }

  for (size_t i = 0; i < size; ++i) {
    file << arr[i];  // Записываем элемент в файл
    if (i != size - 1) {
      file << " ";  // Разделяем элементы пробелом
    }
  }

  file.close();  // Закрываем файл
  std::cout << "Data successfully written to " << filename << std::endl;
}

template <typename T>
void Array<T>::fillFromFile(const std::string& filename) {
  std::ifstream file(filename);  // Открываем файл для чтения
  if (!file.is_open()) {
    std::cerr << "Failed to open file for reading!" << std::endl;
    return;
  }

  T element;
  while (file >> element) {
    emplace_back(element);  // Добавляем элемент в массив
  }

  file.close();  // Закрываем файл
  std::cout << "Data successfully loaded from " << filename << std::endl;
}

template <typename T>
void Array<T>::eraseByValue(const T& value) {
  for (size_t i = 0; i < size; ++i) {
    if (arr[i] == value) {
      erase(i);  // Используем уже существующий метод erase по индексу
      return;  // Удаляем только первый найденный элемент
    }
  }
  std::cerr << "Значение '" << value << "' не найдено в массиве.\n";
}
