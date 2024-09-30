#include <iostream>

using namespace std;

template <typename T>
struct List {
 public:
  List() : size(0), head(nullptr) {}

  ~List() {}

  void push_back(T data) {
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

  void push_front(T data) {
    if (head == nullptr) {
      head = new Node(data);
    } else {
      Node* newHead = new Node(data);

      newHead->pNext = head;

      head = newHead;
    }
    size++;
  }

  int GetSize(){return size}

  T& operator[](size_t index) {
    int counterIndex = 0;
    Node* current = head;

    while (current != nullptr) {
      if (counterIndex == index) {
        return current->data;
      } else {
        current = current->pNext;
        counterIndex++;
      }
    }
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
  List<int> lst;
  lst.push_back(5);
  lst.push_back(10);
  lst.push_back(11);

  cout << lst.GetSize();

  return 0;
}
