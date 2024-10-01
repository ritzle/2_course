

// FIFO

#include <iostream>

using namespace std;

template <typename T>
struct Queue {
 public:
  Queue() : size(0), head(nullptr) {}

  ~Queue() {}

  int GetSize() { return size; }

  void push(T data) {
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

  void pop() {
    if (size == 0) {
      throw invalid_argument("Node is empty");
    }

    if (size == 1) {
      delete head;
      head = nullptr;
    } else {
      Node* current = head;
      head = current->pNext;
      delete (current);
    }
    size--;
  }

  void print() {
    Node* current = head;
    while (current != nullptr) {
      cout << current->data << " ";
      current = current->pNext;
    }
    cout << endl;
  }

 private:
  struct Node {
    Node* pNext;
    T data;

    Node(T data = T(), Node* pNext = nullptr) : data(data), pNext(pNext) {}
  };

  Node* head;
  int size;
};

int main() {
  Queue<int> queue;
  return 0;
}
