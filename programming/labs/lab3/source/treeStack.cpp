#include "../headers/treeStack.hpp"

// Реализация методов стека
template <typename T>
Stack<T>::~Stack() {
  while (!isEmpty()) {
    pop();
  }
}

template <typename T>
void Stack<T>::push(T value) {
  StackNode* newNode = new StackNode(value);
  newNode->next = topNode;
  topNode = newNode;
}

template <typename T>
void Stack<T>::pop() {
  if (!isEmpty()) {
    StackNode* temp = topNode;
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

// Конструктор и деструктор бинарного дерева
template <typename T>
BinaryTreeStack<T>::BinaryTreeStack() : root(nullptr) {}

template <typename T>
BinaryTreeStack<T>::~BinaryTreeStack() {
  clear(root);
}

// Вставка элемента
template <typename T>
void BinaryTreeStack<T>::insert(T value) {
  insert(root, value);
}

template <typename T>
void BinaryTreeStack<T>::insert(Node*& node, T value) {
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
bool BinaryTreeStack<T>::search(T value) const {
  return search(root, value);
}

template <typename T>
bool BinaryTreeStack<T>::search(Node* node, T value) const {
  if (node == nullptr) return false;
  if (node->data == value) return true;
  return value < node->data ? search(node->left, value)
                            : search(node->right, value);
}

// Очистка дерева
template <typename T>
void BinaryTreeStack<T>::clear(Node* node) {
  if (node) {
    clear(node->left);
    clear(node->right);
    delete node;
  }
}

// Итеративный симметричный обход дерева с использованием стека
template <typename T>
void BinaryTreeStack<T>::inorderIterative() const {
  if (!root) return;

  Stack<Node*> stack;
  Node* current = root;

  while (current != nullptr || !stack.isEmpty()) {
    // Идем в самый левый узел
    while (current != nullptr) {
      stack.push(current);
      current = current->left;
    }

    // Выводим верхний элемент из стека
    current = stack.top();
    stack.pop();

    std::cout << current->data << " ";

    // Переходим к правому узлу
    current = current->right;
  }
}

// Проверка баланса дерева
template <typename T>
bool BinaryTreeStack<T>::isBalanced() const {
  if (!root) return true;

  Stack<std::pair<Node*, int>> stack;
  stack.push(std::make_pair(root, 0));
  int minHeight = INT_MAX, maxHeight = INT_MIN;

  while (!stack.isEmpty()) {
    auto [current, level] = stack.top();
    stack.pop();

    if (!current->left && !current->right) {
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
