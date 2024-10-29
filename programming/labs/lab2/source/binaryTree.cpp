#include "../headers/binaryTree.hpp"

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
  if (!node) {
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
  if (!node) return false;
  if (node->data == value) return true;
  if (value < node->data) return search(node->left, value);
  return search(node->right, value);
}

// Обход в симметричном порядке
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

// Очистка памяти
template <typename T>
void BinaryTree<T>::clear(Node* node) {
  if (node) {
    clear(node->left);
    clear(node->right);
    delete node;
  }
}

// Вычисление высоты узла
template <typename T>
int BinaryTree<T>::height(Node* node) const {
  if (!node) return 0;
  return std::max(height(node->left), height(node->right)) + 1;
}

// Проверка баланса узла
template <typename T>
bool BinaryTree<T>::isBalanced(Node* node) const {
  if (!node) return true;

  int leftHeight = height(node->left);
  int rightHeight = height(node->right);

  return (std::abs(leftHeight - rightHeight) <= 1) && isBalanced(node->left) &&
         isBalanced(node->right);
}

// Проверка баланса дерева
template <typename T>
bool BinaryTree<T>::isBalanced() const {
  return isBalanced(root);
}
