#include "../headers/CompleteBinaryTree.h"

using namespace std;

// Конструктор дерева
CompleteBinaryTree::CompleteBinaryTree(int size)
    : root(nullptr), size(size), currentIndex(0) {}

CompleteBinaryTree::Node::Node(int value)
    : data(value), left(nullptr), right(nullptr) {}

CompleteBinaryTree::NodeQueue::NodeQueue(Node* value)
    : data(value), next(nullptr) {}

// Вставка элемента в дерево
void CompleteBinaryTree::insert(int value) {
  if (currentIndex < size) {
    Node* newNode = new Node(value);
    if (root == nullptr) {
      root = newNode;
    } else {
      CustomQueue q;
      q.enqueue(root);
      while (!q.isEmpty()) {
        Node* current = q.peek();
        q.dequeue();
        if (current->left == nullptr) {
          current->left = newNode;
          break;
        } else {
          q.enqueue(current->left);
        }
        if (current->right == nullptr) {
          current->right = newNode;
          break;
        } else {
          q.enqueue(current->right);
        }
      }
    }
    currentIndex++;
  } else {
    std::cout << "Tree is full, cannot insert " << value << std::endl;
  }
}

// Поиск элемента в дереве
bool CompleteBinaryTree::search(int value) { return search(root, value); }

// Внутренняя рекурсивная функция поиска
bool CompleteBinaryTree::search(Node* node, int value) {
  if (node == nullptr) return false;
  if (node->data == value) return true;
  return search(node->left, value) || search(node->right, value);
}

// Проверка полноты дерева
bool CompleteBinaryTree::isComplete() {
  return isComplete(root, 0, currentIndex);
}

// Внутренняя функция проверки полноты
bool CompleteBinaryTree::isComplete(Node* node, int index, int totalNodes) {
  if (node == nullptr) return true;
  if (index >= totalNodes) return false;
  return isComplete(node->left, 2 * index + 1, totalNodes) &&
         isComplete(node->right, 2 * index + 2, totalNodes);
}

// Печать дерева по уровням
void CompleteBinaryTree::printLevelOrder() {
  if (root == nullptr) return;

  CustomQueue queue;
  queue.enqueue(root);
  int currentLevelCount = 1;
  int nextLevelCount = 0;

  while (!queue.isEmpty()) {
    Node* current = queue.peek();
    queue.dequeue();
    std::cout << current->data << " ";

    if (current->left) {
      queue.enqueue(current->left);
      nextLevelCount++;
    }
    if (current->right) {
      queue.enqueue(current->right);
      nextLevelCount++;
    }

    if (--currentLevelCount == 0) {
      cout << endl;
      currentLevelCount = nextLevelCount;
      nextLevelCount = 0;
    }
  }
}

// Метод заполнения дерева из файла
void CompleteBinaryTree::fillTreeFromFile(const std::string& filename) {
  CustomQueue queue;
  queue.fillFromFile(filename);  // Use the CustomQueue's fillFromFile

  // Insert values from the queue into the tree
  while (!queue.isEmpty()) {
    insert(queue.peek()->data);  // Insert the data from the front of the queue
    queue.dequeue();             // Remove the item from the queue
  }
}

// Удаление дерева
CompleteBinaryTree::~CompleteBinaryTree() { destroyTree(root); }

void CompleteBinaryTree::destroyTree(Node* node) {
  if (node) {
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
  }
}

// Конструктор очереди
CompleteBinaryTree::CustomQueue::CustomQueue()
    : front(nullptr), rear(nullptr), size(0) {}

// Деструктор очереди
CompleteBinaryTree::CustomQueue::~CustomQueue() {
  while (!isEmpty()) {
    dequeue();
  }
}

// Добавление элемента в очередь
void CompleteBinaryTree::CustomQueue::enqueue(Node* value) {
  NodeQueue* newNode = new NodeQueue(value);
  if (rear == nullptr) {
    front = rear = newNode;
  } else {
    rear->next = newNode;
    rear = newNode;
  }
  size++;
}

// Удаление элемента из очереди
void CompleteBinaryTree::CustomQueue::dequeue() {
  if (isEmpty()) {
    throw out_of_range("Queue is empty");
  }
  NodeQueue* temp = front;
  front = front->next;
  if (front == nullptr) {
    rear = nullptr;
  }
  delete temp;
  size--;
}

// Получение первого элемента очереди
CompleteBinaryTree::Node* CompleteBinaryTree::CustomQueue::peek() {
  if (isEmpty()) {
    throw out_of_range("Queue is empty");
  }
  return front->data;
}

// Проверка на пустоту очереди
bool CompleteBinaryTree::CustomQueue::isEmpty() { return front == nullptr; }

// Получение размера очереди
int CompleteBinaryTree::CustomQueue::getSize() { return size; }

// Метод заполнения из файла
void CompleteBinaryTree::CustomQueue::fillFromFile(
    const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open the file: " << filename << std::endl;
    return;
  }

  int value;  // Переменная для хранения значения
  while (file >> value) {
    enqueue(
        new Node(value));  // Используйте enqueue для добавления узла в очередь
  }

  file.close();
}
