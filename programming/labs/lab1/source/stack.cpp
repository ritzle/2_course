#include "../headers/stack.h"

template <typename T>
Stack<T>::Stack() : size(0), head(nullptr) {}

template <typename T>
Stack<T>::~Stack() {
  while (size > 0) {
    pop();  // Удаляем все элементы стека при уничтожении объекта
  }
}

template <typename T>
int Stack<T>::GetSize() {
  return size;
}

template <typename T>
void Stack<T>::push(T data) {
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
void Stack<T>::pop() {
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
void Stack<T>::print() {
  Node* current = head;
  while (current != nullptr) {
    cout << current->data << " ";
    current = current->pNext;
  }
  cout << endl;
}

template <typename T>
void Stack<T>::fillFromFile(const std::string& filename) {
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
