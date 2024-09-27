#include <iostream>

using namespace std;

template <typename T>
struct MyArray {
  T *arr;

 private:
  size_t size;  // отслеживает количество элементов
  size_t capacity;  // вместимость

  void resize(size_t new_capacity) {
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

 public:
  MyArray(size_t init_capacity) : size(0), capacity(init_capacity) {
    if (init_capacity == 0) {
      throw length_error("length error");
    }
    arr = new T[init_capacity]();
  }

  ~MyArray() { delete[] arr; }

  T &operator[](size_t index) {
    if (index >= size) {
      throw out_of_range("Index out of bounds");
    }
    return arr[index];
  }

  void emplace_back(T element) {
    if (size == capacity) {
      resize(capacity * 2);
    }

    arr[size] = element;
    size++;
  };

  void insert(size_t indexInput, T element) {
    if (indexInput >= capacity || indexInput < 0) {
      throw invalid_argument("there is no such position");
    }

    if (size == capacity) {
      resize(capacity * 2);  // Увеличиваем размер массива
    }

    if (indexInput == size) {
      arr[size] = element;
    } else {
      for (size_t i = size; i > indexInput; --i) {
        arr[i] = arr[i - 1];
      }

      arr[indexInput] = element;
    }

    size++;
  }

  void erase(size_t indexInput) {
    if (indexInput >= capacity || indexInput < 0) {
      throw invalid_argument("нет такой позиции");
    }

    if (indexInput == size) {
      arr[size] = 0;
    } else {
      for (size_t i = indexInput; i < size; i++) {
        arr[i] = arr[i + 1];
      }
    }

    if (size != 0) {
      size--;
    }
  }

  void replace(size_t indexInput, T element) {
    if (indexInput >= capacity || indexInput < 0) {
      throw invalid_argument("нет такой позиции");
    }

    if (size <= indexInput) {
      size = indexInput;
    }

    if (indexInput == 0) {
      size++;
    }

    arr[indexInput] = element;
  }

  void getSize() { cout << capacity << endl; }

  void print() {
    for (int i = 0; i < size; i++) {
      cout << arr[i] << " ";
    }
    cout << endl;
  }
};

int main() {
  MyArray<int> arr{5};
  return 0;
}