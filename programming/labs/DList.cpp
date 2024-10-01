#include <iostream>
using namespace std;

template <typename T>
struct DList {
 public:
  DList() : size(0), head(nullptr), tail(nullptr) {}

  ~DList() {}

  int GetSize() { return size; }

  void push_back(T data) {
    Node* current = new Node(data);
    if (head == nullptr) {
      head = current;
      tail = current;
    } else {
      tail->pNext = current;
      current->pPrev = tail;
      tail = current;
    }
    size++;
  }

  void push_front(T data) {
    Node* current = new Node(data);
    if (head == nullptr) {
      head = current;
      tail = current;
    } else {
      current->pNext = head;
      head->pPrev = current;
      head = current;
    }
    size++;
  }

  void pop_front() {
    if (size == 0) {
      throw invalid_argument("List is empty");
    }

    if (size == 1) {
      delete head;
      head = nullptr;
      tail = nullptr;
    } else {
      Node* temp = head;
      head = head->pNext;
      head->pPrev = nullptr;
      delete temp;
    }
    size--;
  }

  void pop_back() {
    if (size == 0) {
      throw invalid_argument("List is empty");
    }

    if (size == 1) {
      delete head;
      head = nullptr;
      tail = nullptr;
    } else {
      Node* temp = tail;
      tail = tail->pPrev;
      tail->pNext = nullptr;
      delete temp;
    }

    size--;
  }

  void pop_value(T searchData) {
    if (size == 0) {
      throw invalid_argument("List is empty");
    }

    // Удаление элементов с начала списка, если они соответствуют значению
    while (head != nullptr && head->data == searchData) {
      Node* temp = head;
      head = head->pNext;
      if (head) {
        head->pPrev = nullptr;
      } else {
        tail = nullptr;
      }
      delete temp;
      size--;
    }

    // Для всех остальных элементов
    Node* current = head;
    while (current != nullptr) {
      if (current->data == searchData) {
        Node* temp = current;

        if (current->pPrev != nullptr) {
          current->pPrev->pNext = current->pNext;
        }
        if (current->pNext != nullptr) {
          current->pNext->pPrev = current->pPrev;
        }

        if (current == tail) {
          tail = current->pPrev;
        }

        current = current->pNext;
        delete temp;
        size--;
      } else {
        current = current->pNext;
      }
    }
  }

  void search_by_value(T searchData) {
    int index = 0;
    Node* current = head;
    bool found = false;

    cout << "index value " << searchData << ": ";

    while (current != nullptr) {
      if (current->data == searchData) {
        cout << index << " ";
        found = true;
      }
      current = current->pNext;
      index++;
    }

    if (!found) {
      cout << "not found";
    }

    cout << endl;
  }

  T& operator[](size_t index) {
    if (index >= size) {
      throw out_of_range("Index out of bounds");
    }

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
    Node* pPrev;
    T data;

    Node(T data = T(), Node* pNext = nullptr, Node* pPrev = nullptr)
        : data(data), pNext(pNext), pPrev(pPrev) {}
  };

  Node* head;
  Node* tail;
  int size;
};

int main() {
  DList<int> lst;
  lst.push_back(5);
  lst.push_back(10);
  lst.push_back(11);
  lst.push_back(11);
  lst.push_back(11);

  lst.pop_value(10);

  lst.search_by_value(11);

  lst.print();

  return 0;
}
