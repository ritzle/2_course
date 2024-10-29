#include "../headers/stack.hpp"

// Реализация стека
template <typename T>
Stack<T>::~Stack() {
  while (!isEmpty()) {
    pop();
  }
}

template <typename T>
void Stack<T>::push(T value) {
  Node* newNode = new Node(value);
  newNode->next = topNode;
  topNode = newNode;
}

template <typename T>
void Stack<T>::pop() {
  if (!isEmpty()) {
    Node* temp = topNode;
    topNode = topNode->next;
    delete temp;
  }
}

template <typename T>
T Stack<T>::top() const {
  if (!isEmpty()) {
    return topNode->data;
  }
  throw std::runtime_error("Stack is empty");
}

template <typename T>
bool Stack<T>::isEmpty() const {
  return topNode == nullptr;
}

// Конструктор
template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

// Деструктор
template <typename T>
BinaryTree<T>::~BinaryTree() {
  clear(root);
}

// Вставка элемента
template <typename T>
void BinaryTree<T>::insert(T value) {
  insert(root, value);
}

template <typename T>
void BinaryTree<T>::insert(Node*& node, T value) {
  if (node == nullptr) {
    node = new Node(value);
  } else if (value < node->data) {
    insert(node->left, value);
  } else {
    insert(node->right, value);
  }
}

// Поиск элемента
template <typename T>
bool BinaryTree<T>::search(T value) const {
  return search(root, value);
}

template <typename T>
bool BinaryTree<T>::search(Node* node, T value) const {
  if (node == nullptr) return false;
  if (node->data == value) return true;
  return value < node->data ? search(node->left, value)
                            : search(node->right, value);
}

// Обход дерева в симметричном порядке
template <typename T>
void BinaryTree<T>::inorder() const {
  inorder(root);
}

template <typename T>
void BinaryTree<T>::inorder(Node* node) const {
  if (node) {
    inorder(node->left);
    std::cout << node->data << " ";
    inorder(node->right);
  }
}

// Освобождение памяти
template <typename T>
void BinaryTree<T>::clear(Node* node) {
  if (node) {
    clear(node->left);
    clear(node->right);
    delete node;
  }
}

// Проверка баланса
template <typename T>
bool BinaryTree<T>::isBalanced() const {
  if (!root) return true;

  Stack<std::pair<Node*, int>> stack;
  stack.push(std::make_pair(root, 0));
  int minHeight = INT_MAX, maxHeight = INT_MIN;

  while (!stack.isEmpty()) {
    auto [current, level] = stack.top();
    stack.pop();

    if (!current->left && !current->right) {
      // Листовой узел
      minHeight = std::min(minHeight, level);
      maxHeight = std::max(maxHeight, level);
    }

    if (current->left) {
      stack.push(std::make_pair(current->left, level + 1));
    }
    if (current->right) {
      stack.push(std::make_pair(current->right, level + 1));
    }
  }

  return (maxHeight - minHeight) <= 1;
}
