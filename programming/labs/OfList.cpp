#include <iostream>

using namespace std;

template <typename T>
struct OList {
 public:
  OList() : size(0), head(nullptr) {}

  ~OList() {}

  int GetSize() { return size; }

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

  void pop_front() {
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

  void pop_back() {
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

  void pop_value(T searchData) {
    // Проверка, если список пуст
    if (size == 0) {
      throw invalid_argument("List is empty");
    }

    // Удаление элемента, если он находится в начале списка
    while (head != nullptr && head->data == searchData) {
      Node* temp = head;
      head = head->pNext;
      delete temp;
      size--;
    }

    // Для всех остальных элементов
    Node* current = head;
    while (current != nullptr && current->pNext != nullptr) {
      if (current->pNext->data == searchData) {
        Node* temp = current->pNext;
        current->pNext = current->pNext->pNext;
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
  OList<int> lst;
  lst.push_back(5);
  lst.push_back(10);
  lst.push_back(11);
  lst.push_back(11);
  lst.push_back(11);

  lst.pop_value(10);

  lst.search_by_value(11);

  // cout << lst.GetSize() << endl;

  lst.print();

  return 0;
}
