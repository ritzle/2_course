#include "../headers/OList.h"

#include <stdexcept>

using namespace std;

template <typename T>
OList<T>::OList() : size(0), head(nullptr) {}

template <typename T>
OList<T>::~OList() {
  while (head != nullptr) {
    pop_front();  // Удаляем все элементы
  }
}

template <typename T>
int OList<T>::GetSize() {
  return size;
}

template <typename T>
void OList<T>::push_back(T data) {
  if (head == nullptr) {
    head = new Node(data);
  } else {
    Node* current = head;
    while (current->pNext != nullptr) {
      current = current->pNext;
    }
    current->pNext = new Node(data);
  }
  size++;
}

template <typename T>
void OList<T>::push_front(T data) {
  Node* newHead = new Node(data, head);
  head = newHead;
  size++;
}

template <typename T>
void OList<T>::pop_front() {
  if (size == 0) {
    throw invalid_argument("Node is empty");
  }

  Node* temp = head;
  head = head->pNext;
  delete temp;
  size--;
}

template <typename T>
void OList<T>::pop_back() {
  if (size == 0) {
    throw invalid_argument("List is empty");
  }

  if (size == 1) {
    delete head;
    head = nullptr;
  } else {
    Node* current = head;
    while (current->pNext->pNext != nullptr) {
      current = current->pNext;
    }
    delete current->pNext;
    current->pNext = nullptr;
  }
  size--;
}

template <typename T>
void OList<T>::pop_value(T searchData) {
  if (size == 0) {
    throw invalid_argument("List is empty");
  }

  while (head != nullptr && head->data == searchData) {
    Node* temp = head;
    head = head->pNext;
    delete temp;
    size--;
  }

  Node* current = head;
  while (current != nullptr && current->pNext != nullptr) {
    if (current->pNext->data == searchData) {
      Node* temp = current->pNext;
      current->pNext = current->pNext->pNext;
      delete temp;
      size--;
    } else {
      current = current->pNext;
    }
  }
}

template <typename T>
void OList<T>::search_by_value(T searchData) {
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
T& OList<T>::operator[](size_t index) {
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
void OList<T>::print() {
  Node* current = head;
  while (current != nullptr) {
    cout << current->data << " ";
    current = current->pNext;
  }
  cout << endl;
}

template <typename T>
void OList<T>::fillFromFile(const std::string& filename) {
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