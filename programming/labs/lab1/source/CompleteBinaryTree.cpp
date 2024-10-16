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

#include <iomanip>  // Для std::setw

void CompleteBinaryTree::printLevelOrder() {
  if (root == nullptr) return;

  int treeHeight = height();  // Получаем высоту дерева
  CustomQueue queue;
  queue.enqueue(root);
  int currentLevelCount = 1;  // Количество узлов на текущем уровне
  int nextLevelCount = 0;  // Количество узлов на следующем уровне

  for (int level = 0; level < treeHeight; ++level) {
    int spaces = pow(2, treeHeight - level) - 1;  // Пробелы перед узлом

    // Печатаем пробелы перед узлами на уровне
    for (int i = 0; i < spaces; ++i) {
      std::cout << " ";
    }

    // Печатаем узлы на текущем уровне
    for (int i = 0; i < currentLevelCount; ++i) {
      Node* current = queue.peek();
      queue.dequeue();

      std::cout << current->data;  // Печать значения узла

      // Если у узла есть дочерние узлы, добавляем их в очередь
      if (current->left) {
        queue.enqueue(current->left);
        nextLevelCount++;
      }
      if (current->right) {
        queue.enqueue(current->right);
        nextLevelCount++;
      }

      // Печатаем пробелы между узлами
      if (i <
          currentLevelCount - 1) {  // Не добавляем пробел после последнего узла
        // Увеличиваем расстояние между узлами для лучшего форматирования
        for (int j = 0; j < spaces * 2 + 1;
             ++j) {  // Adjust spacing for better alignment
          std::cout << " ";
        }
      }
    }

    std::cout << std::endl;  // Переход на новую строку
    currentLevelCount =
        nextLevelCount;  // Обновляем количество узлов текущего уровня
    nextLevelCount = 0;  // Сбрасываем количество узлов следующего уровня
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

// Запись дерева в файл
void CompleteBinaryTree::writeToFile(const std::string& filename) {
  std::ofstream outFile(filename);
  if (!outFile) {
    std::cerr << "Не удалось открыть файл для записи: " << filename
              << std::endl;
    return;
  }

  writeToFile(root, outFile);
}

void CompleteBinaryTree::writeToFile(Node* node, std::ofstream& outFile) {
  if (node) {
    outFile << node->data << " ";
    writeToFile(node->left, outFile);
    writeToFile(node->right, outFile);
  }
}

int CompleteBinaryTree::height(Node* node) {
  if (node == nullptr) {
    return 0;  // Высота пустого дерева - 0
  }
  // Рекурсивно вычисляем высоту левого и правого поддеревьев
  int leftHeight = height(node->left);
  int rightHeight = height(node->right);
  // Возвращаем максимальную высоту
  return std::max(leftHeight, rightHeight) + 1;
}

int CompleteBinaryTree::height() {
  return height(root);  // Вызываем рекурсивный метод от корня
}
