#include "../headers/DList.h"

#include <stdexcept>

using namespace std;

template <typename T>
DList<T>::DList() : size(0), head(nullptr), tail(nullptr) {}

template <typename T>
DList<T>::~DList() {
  while (size > 0) {
    pop_front();  // Удаляем все элементы
  }
}

template <typename T>
int DList<T>::GetSize() {
  return size;
}

template <typename T>
void DList<T>::push_back(T data) {
  Node* current = new Node(data);
  if (head == nullptr) {
    head = current;
    tail = current;
  } else {
    tail->pNext = current;
    current->pPrev = tail;
    tail = current;
  }
  size++;
}

template <typename T>
void DList<T>::push_front(T data) {
  Node* current = new Node(data);
  if (head == nullptr) {
    head = current;
    tail = current;
  } else {
    current->pNext = head;
    head->pPrev = current;
    head = current;
  }
  size++;
}

template <typename T>
void DList<T>::pop_front() {
  if (size == 0) {
    throw invalid_argument("List is empty");
  }

  Node* temp = head;
  head = head->pNext;
  if (head) {
    head->pPrev = nullptr;
  } else {
    tail = nullptr;  // Если список стал пустым
  }
  delete temp;
  size--;
}

template <typename T>
void DList<T>::pop_back() {
  if (size == 0) {
    throw invalid_argument("List is empty");
  }

  Node* temp = tail;
  tail = tail->pPrev;
  if (tail) {
    tail->pNext = nullptr;
  } else {
    head = nullptr;  // Если список стал пустым
  }
  delete temp;
  size--;
}

template <typename T>
void DList<T>::pop_value(T searchData) {
  if (size == 0) {
    throw invalid_argument("List is empty");
  }

  while (head != nullptr && head->data == searchData) {
    Node* temp = head;
    head = head->pNext;
    if (head) {
      head->pPrev = nullptr;
    } else {
      tail = nullptr;  // Если список стал пустым
    }
    delete temp;
    size--;
  }

  Node* current = head;
  while (current != nullptr) {
    if (current->data == searchData) {
      Node* temp = current;
      if (current->pPrev != nullptr) {
        current->pPrev->pNext = current->pNext;
      }
      if (current->pNext != nullptr) {
        current->pNext->pPrev = current->pPrev;
      }
      if (current == tail) {
        tail = current->pPrev;
      }
      current = current->pNext;
      delete temp;
      size--;
    } else {
      current = current->pNext;
    }
  }
}

template <typename T>
void DList<T>::search_by_value(T searchData) {
  int index = 0;
  Node* current = head;
  bool found = false;

  cout << "index value " << searchData << ": ";

  while (current != nullptr) {
    if (current->data == searchData) {
      cout << index << " ";
      found = true;
    }
    current = current->pNext;
    index++;
  }

  if (!found) {
    cout << "not found";
  }

  cout << endl;
}

template <typename T>
T& DList<T>::operator[](size_t index) {
  if (index >= size) {
    throw out_of_range("Index out of bounds");
  }

  int counterIndex = 0;
  Node* current = head;

  while (current != nullptr) {
    if (counterIndex == index) {
      return current->data;
    } else {
      current = current->pNext;
      counterIndex++;
    }
  }

  throw out_of_range("Index out of bounds");  // Если индекс не найден
}

template <typename T>
void DList<T>::print() {
  Node* current = head;
  while (current != nullptr) {
    cout << current->data << " ";
    current = current->pNext;
  }
  cout << endl;
}

template <typename T>
void DList<T>::fillFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open the file: " << filename << std::endl;
    return;
  }

  T value;
  while (file >> value) {
    push_back(value);
  }

  file.close();
}
