
// LIFO

#include <iostream>

using namespace std;

template <typename T>
struct Stack {
 public:
  Stack() : size(0), head(nullptr) {}

  ~Stack() {}

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

int main() { return 0; }
