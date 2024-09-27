#include <iostream>

using namespace std;

template <typename T>
struct Node {
  T value;
  Node* next;
  Node(T val) : value(val), next(nullptr){};
};

template <typename T>
struct LinkedList {
 private:
  Node<T>* head{nullptr};
  int sizeList = 0;
  int index = 0;

  bool IsValidValue(T item) {
    Node<T>* current = head;
    while (current != nullptr) {
      if (current->value == item) {
        return true;
      }
      current = current->next;
    }
    return false;
  }

 public:
  int GetQuantity() { return sizeList; }
  int GetIndex() { return index; }

  void InsertToBack(T item) {
    if (head == nullptr) {
      head = new Node<T>(item);
      sizeList++;
    } else {
      Node<T>* current = head;
      while (current->next != nullptr) {
        current = current->next;
      }
      current->next = new Node<T>(item);
      sizeList++;
    }
  }

  void InsertToStart(T item) {
    if (head == nullptr) {
      head = new Node<T>(item);
      sizeList++;
    } else {
      Node<T>* newNode = new Node<T>(item);
      newNode->next = head;
      head = newNode;
      sizeList++;
    }
  }

  void SearchByValue(T item) {
    Node<T>* current = head;
    while (current != nullptr) {
      if (current->value == item) {
        cout << "Found value: " << current->value << " Index: " << index
             << endl;
        return;
      }
      current = current->next;
      index++;
    }
    cerr << "Error: Item not found" << endl;
  }

  void DeleteToBack() {
    Node<T>* currentPtr = head;
    Node<T>* prevPtr = nullptr;
    if (currentPtr == nullptr) {
      cerr << "Error. Empty list" << endl;
      return;
    } else if (currentPtr->next == nullptr) {
      delete currentPtr;
      head = nullptr;
      sizeList--;
      return;
    }

    while (currentPtr->next != nullptr) {
      prevPtr = currentPtr;
      currentPtr = currentPtr->next;
    }

    free(currentPtr);
    prevPtr->next = nullptr;
    sizeList--;
  }

  void DeleteToStart() {
    Node<T>* currentPtr = head;
    Node<T>* nextHead = currentPtr->next;

    if (currentPtr == nullptr) {
      cerr << "Error. Empty list" << endl;
      return;
    }

    head = nextHead;
    delete currentPtr;
    sizeList--;
  }

  void DeleteByValue(T item) {
    Node<T>* currentPtr = head;
    Node<T>* prevPtr = nullptr;

    if (IsValidValue(item) == false) {
      cerr << "Item not found" << endl;
      return;
    }
    if (currentPtr == nullptr) {
      cerr << "Empty list" << endl;  // список пуст
      return;
    } else if (currentPtr->next == nullptr) {
      delete head;
      delete currentPtr;
      sizeList--;
      return;
    }

    while (currentPtr != nullptr) {
      if (currentPtr->value == item &&
          currentPtr == head)  // если значение голова списка
      {
        DeleteToStart();
        sizeList--;
        return;
      } else if (currentPtr->value == item) {
        prevPtr->next = currentPtr->next;
        delete currentPtr;
        sizeList--;
        return;
      }
      prevPtr = currentPtr;
      currentPtr = currentPtr->next;
    }
  }

  void PrintList() {
    Node<T>* current = head;
    if (current == nullptr) {
      cout << "Empty list" << endl;
    }
    while (current != nullptr) {
      cout << current->value << " ";
      current = current->next;
    }
    cout << endl;
  }
};

int main() {
  LinkedList<int> intList;

  return 0;
}