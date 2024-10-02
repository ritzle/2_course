#include "../headers/queue.h"

#include <stdexcept>

using namespace std;

template <typename T>
Queue<T>::Queue() : size(0), head(nullptr) {}

template <typename T>
Queue<T>::~Queue() {
  while (size > 0) {
    pop();  // Удаляем все элементы
  }
}

template <typename T>
int Queue<T>::GetSize() {
  return size;
}

template <typename T>
void Queue<T>::push(T data) {
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
void Queue<T>::pop() {
  if (size == 0) {
    throw invalid_argument("Node is empty");
  }

  Node* current = head;
  head = current->pNext;
  delete current;
  size--;

  if (size == 0) {
    head = nullptr;  // Устанавливаем head в nullptr, если очередь пуста
  }
}

template <typename T>
void Queue<T>::print() {
  Node* current = head;
  while (current != nullptr) {
    cout << current->data << " ";
    current = current->pNext;
  }
  cout << endl;
}

template <typename T>
void Queue<T>::fillFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open the file: " << filename << std::endl;
    return;
  }

  T value;
  while (file >> value) {
    push(value);  // Используем метод push для добавления элемента в очередь
  }

  file.close();
}
