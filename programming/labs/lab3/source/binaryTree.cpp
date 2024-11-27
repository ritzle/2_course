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

#include <fstream>
#include <stdexcept>

template <typename T>
void BinaryTree<T>::serialize(const std::string& filename) const {
  std::ofstream ofs(filename, std::ios::binary);
  if (!ofs.is_open()) {
    throw std::runtime_error("Failed to open file for writing");
  }

  serializeHelper(ofs, root);
  ofs.close();
}

template <typename T>
void BinaryTree<T>::serializeHelper(std::ofstream& ofs, Node* node) const {
  if (!node) {
    // Записываем "нулевой" узел для обозначения конца поддерева (или пустого
    // узла)
    ofs.write(reinterpret_cast<const char*>(&node), sizeof(Node*));
  } else {
    // Записываем текущий узел
    ofs.write(reinterpret_cast<const char*>(&node->data), sizeof(T));
    serializeHelper(ofs, node->left);
    serializeHelper(ofs, node->right);
  }
}

template <typename T>
void BinaryTree<T>::deserialize(const std::string& filename) {
  std::ifstream ifs(filename, std::ios::binary);
  if (!ifs.is_open()) {
    throw std::runtime_error("Failed to open file for reading");
  }

  root = deserializeHelper(ifs);
  ifs.close();
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::deserializeHelper(
    std::ifstream& ifs) {
  T value;
  ifs.read(reinterpret_cast<char*>(&value), sizeof(T));

  // Если узел пустой, возвращаем nullptr
  if (ifs.eof()) {
    return nullptr;
  }

  Node* node = new Node(value);
  node->left = deserializeHelper(ifs);
  node->right = deserializeHelper(ifs);

  return node;
}
