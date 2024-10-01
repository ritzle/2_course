#include <iostream>
#include <queue>  // Добавляем библиотеку для использования очереди

using namespace std;

struct CompleteBinaryTree {
 public:
  CompleteBinaryTree(int size) : root(nullptr), size(size), currentIndex(0) {}

  void insert(int value) {
    if (currentIndex < size) {
      Node* newNode = new Node(value);
      if (root == nullptr) {
        root = newNode;  // Устанавливаем корень
      } else {
        queue<Node*> q;  // Используем очередь для уровня
        q.push(root);
        while (!q.empty()) {
          Node* current = q.front();
          q.pop();

          // Добавляем в левый дочерний узел
          if (current->left == nullptr) {
            current->left = newNode;
            break;
          } else {
            q.push(current->left);
          }

          // Добавляем в правый дочерний узел
          if (current->right == nullptr) {
            current->right = newNode;
            break;
          } else {
            q.push(current->right);
          }
        }
      }
      currentIndex++;
    } else {
      std::cout << "Tree is full, cannot insert " << value << std::endl;
    }
  }

  bool search(int value) { return search(root, value); }

  bool isComplete() { return isComplete(root, 0, currentIndex); }

  ~CompleteBinaryTree() { destroyTree(root); }

  void printLevelOrder() {
    if (root == nullptr) return;

    Node** queue = new Node*[size];  // Максимальный размер известен
    int front = 0, rear = 0;

    queue[rear++] = root;  // Добавляем корень в очередь
    int currentLevelCount = 1;  // Количество узлов на текущем уровне
    int nextLevelCount = 0;  // Количество узлов на следующем уровне

    while (front < rear) {
      Node* current = queue[front++];
      std::cout << current->data << " ";

      // Добавляем детей в очередь
      if (current->left) {
        queue[rear++] = current->left;
        nextLevelCount++;
      }
      if (current->right) {
        queue[rear++] = current->right;
        nextLevelCount++;
      }

      // Если достигли конца текущего уровня
      if (--currentLevelCount == 0) {
        cout << endl;
        currentLevelCount = nextLevelCount;
        nextLevelCount = 0;
      }
    }

    delete[] queue;
  }

 private:
  struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) : data(value), left(nullptr), right(nullptr) {}
  };

  Node* root;
  int size;
  int currentIndex;

  bool search(Node* node, int value) {
    if (node == nullptr) return false;
    if (node->data == value) return true;
    return search(node->left, value) || search(node->right, value);
  }

  bool isComplete(Node* node, int index, int totalNodes) {
    if (node == nullptr) return true;
    if (index >= totalNodes) return false;
    return isComplete(node->left, 2 * index + 1, totalNodes) &&
           isComplete(node->right, 2 * index + 2, totalNodes);
  }

  void destroyTree(Node* node) {
    if (node) {
      destroyTree(node->left);
      destroyTree(node->right);
      delete node;
    }
  }
};

int main() {
  CompleteBinaryTree cbt(7);  // Устанавливаем размер дерева

  cbt.insert(1);
  cbt.insert(2);
  cbt.insert(3);
  cbt.insert(4);
  cbt.insert(5);
  cbt.insert(6);
  cbt.insert(7);

  std::cout << "Обход полного бинарного дерева в ширину: \n";
  cbt.printLevelOrder();

  int searchValue = 4;
  if (cbt.search(searchValue)) {
    std::cout << "Элемент " << searchValue << " найден в дереве." << std::endl;
  } else {
    std::cout << "Элемент " << searchValue << " не найден в дереве."
              << std::endl;
  }

  if (cbt.isComplete()) {
    std::cout << "Дерево является полным." << std::endl;
  } else {
    std::cout << "Дерево не является полным." << std::endl;
  }

  return 0;
}
