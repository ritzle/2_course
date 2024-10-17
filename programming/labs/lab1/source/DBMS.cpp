#include "../headers/DBMS.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "../headers/CompleteBinaryTree.h"
#include "../headers/DList.h"
#include "../headers/OList.h"
#include "../headers/array.h"
#include "../headers/mapHash.h"
#include "../headers/queue.h"
#include "../headers/stack.h"

using namespace std;

//--file inpyt.txt --query 'MPUSh 5'
void DBMS::handle(const string& request) {
  istringstream iss(request);
  string fileName, query, token;

  while (iss >> token) {
    if (token == "--file") {
      if (!(iss >> fileName)) {
        cerr << "Ошибка: отсутствует имя файла после --file.\n";
        return;
      }
    } else if (token == "--query") {
      getline(iss, query);
      query.erase(0, query.find_first_not_of(" "));

      query.erase(remove(query.begin(), query.end(), '\''), query.end());
    }
  }

  if (fileName.empty() || query.empty()) {
    cerr << "Некорректный запрос: необходимо указать --file и --query.\n";
    return;
  } else {
    char firstChar = query[0];
    query.erase(0, 1);

    switch (firstChar) {
      case 'M':
        handleArray(fileName, query);
        break;
      case 'D':
        handleDoobleList(fileName, query);
        break;
      case 'L':
        handleList(fileName, query);
        break;
      case 'Q':
        handleQueue(fileName, query);
        break;
      case 'S':
        handleStack(fileName, query);
        break;
      case 'T':
        handleTree(fileName, query);

      default:
        cout << "нет обращение к структуре";
        break;
    }
  }
}

void DBMS::handleArray(const string& fileName, const string& query) {
  Array<string> array;
  array.fillFromFile(fileName);  // Считываем массив из файла

  istringstream iss(query);
  string command;
  string value;
  size_t index;

  iss >> command;

  if (command == "PUSH") {
    iss >> value;
    array.emplace_back(value);
    array.writeToFile(fileName);
    cout << "Добавлено в массив: " << value << "\n";
  } else if (command == "DELINDEX") {
    iss >> index;
    if (index < array.getSize()) {
      array.erase(index);
      array.writeToFile(fileName);
      cout << "Удалено из массива на индексе: " << index << "\n";
    } else {
      cerr << "Индекс " << index << " вне диапазона.\n";
    }
  } else if (command == "DELVAL") {
    iss >> value;               // Получаем значение
    array.eraseByValue(value);  // Удаление элемента по значению
    array.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Удалено из массива значение: " << value << "\n";
  } else if (command == "INTER") {
    iss >> index >> value;  // Получаем индекс и значение
    if (index <= array.getSize()) {
      array.insert(index, value);  // Вставка элемента по индексу
      array.writeToFile(fileName);  // Записываем обратно в файл
      cout << "Вставлено в массив на индексе: " << index
           << ", значение: " << value << "\n";
    } else {
      cerr << "Индекс " << index << " вне диапазона для вставки.\n";
    }
  } else if (command == "REPL") {
    iss >> index >> value;  // Получаем индекс и новое значение
    if (index < array.getSize()) {
      array.replace(index, value);  // Замена элемента по индексу
      array.writeToFile(fileName);  // Записываем обратно в файл
      cout << "Заменено значение на индексе: " << index << " на " << value
           << "\n";
    } else {
      cerr << "Индекс " << index << " вне диапазона для замены.\n";
    }
  } else if (command == "GET") {
    iss >> index;
    if (index < array.getSize()) {
      cout << "Элемент на индексе " << index << ": " << array[index] << "\n";
    } else {
      cerr << "Индекс " << index << " вне диапазона.\n";
    }
  } else {
    cerr << "Неверная команда для массива: " << command << "\n";
  }
}

void DBMS::handleDoobleList(const string& fileName, const string& query) {
  DList<string> list;
  list.fillFromFile(fileName);  // Считываем список из файла

  istringstream iss(query);
  string command;
  string value;

  iss >> command;  // Считываем команду

  if (command == "PUSHBACK") {
    iss >> value;  // Получаем значение
    list.push_back(value);  // Добавляем значение в конец списка
    list.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Добавлено в список: " << value << "\n";
  } else if (command == "PUSHFRONT") {
    iss >> value;  // Получаем значение
    list.push_front(value);  // Добавляем значение в начало списка
    list.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Добавлено в начало списка: " << value << "\n";
  } else if (command == "POPFRONT") {
    list.pop_front();            // Удаляем первый элемент
    list.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Удален первый элемент списка.\n";
  } else if (command == "POPBACK") {
    list.pop_back();  // Удаляем последний элемент
    list.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Удален последний элемент списка.\n";
  } else if (command == "DELVAL") {
    iss >> value;           // Получаем значение
    list.pop_value(value);  // Удаление элемента по значению
    list.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Удалено значение: " << value << "\n";
  } else if (command == "SEARCH") {
    iss >> value;  // Получаем значение для поиска
    list.search_by_value(value);  // Поиск элемента по значению
  } else if (command == "GET") {
    size_t index;
    iss >> index;  // Получаем индекс
    if (index < list.GetSize()) {
      cout << "Элемент на индексе " << index << ": " << list[index] << "\n";
    } else {
      cerr << "Индекс " << index << " вне диапазона.\n";
    }
  } else {
    cerr << "Неверная команда для списка: " << command << "\n";
  }
}

void DBMS::handleList(const string& fileName, const string& query) {
  OList<string> list;
  list.fillFromFile(fileName);  // Считываем список из файла

  istringstream iss(query);
  string command;
  string value;

  iss >> command;  // Получаем команду

  if (command == "PUSH") {
    iss >> value;           // Получаем значение
    list.push_back(value);  // Добавляем значение в список
    list.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Добавлено в список: " << value << "\n";
  } else if (command == "POPFRONT") {
    if (list.GetSize() > 0) {
      list.pop_front();  // Удаляем элемент из начала списка
      list.writeToFile(fileName);  // Записываем обратно в файл
      cout << "Удален из начала списка элемент.\n";
    } else {
      cerr << "Список пуст, ничего не удалено.\n";
    }
  } else if (command == "POPBACK") {
    if (list.GetSize() > 0) {
      list.pop_back();  // Удаляем элемент из конца списка
      list.writeToFile(fileName);  // Записываем обратно в файл
      cout << "Удален из конца списка элемент.\n";
    } else {
      cerr << "Список пуст, ничего не удалено.\n";
    }
  } else if (command == "DELVAL") {
    iss >> value;           // Получаем значение
    list.pop_value(value);  // Удаление элемента по значению
    list.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Удалено из списка значение: " << value << "\n";
  } else if (command == "SEARCH") {
    iss >> value;  // Получаем значение для поиска
    list.search_by_value(value);  // Поиск значения в списке
  } else if (command == "PRINT") {
    list.print();  // Печатаем элементы списка
  } else {
    cerr << "Неверная команда для списка: " << command << "\n";
  }
}

void DBMS::handleQueue(const string& fileName, const string& query) {
  Queue<string> queue;
  queue.fillFromFile(fileName);  // Считываем очередь из файла

  istringstream iss(query);
  string command;
  string value;

  iss >> command;  // Получаем команду

  if (command == "PUSH") {
    iss >> value;       // Получаем значение
    queue.push(value);  // Добавляем значение в очередь
    queue.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Добавлено в очередь: " << value << "\n";
  } else if (command == "POP") {
    if (queue.GetSize() > 0) {
      queue.pop();  // Удаляем элемент из очереди
      queue.writeToFile(fileName);  // Записываем обратно в файл
      cout << "Удален из очереди элемент.\n";
    } else {
      cerr << "Очередь пуста, ничего не удалено.\n";
    }
  } else if (command == "PRINT") {
    queue.print();  // Печатаем элементы очереди
  } else {
    cerr << "Неверная команда для очереди: " << command << "\n";
  }
}

void DBMS::handleStack(const string& fileName, const string& query) {
  Stack<string> stack;
  stack.fillFromFile(fileName);  // Считываем стек из файла

  istringstream iss(query);
  string command;
  string value;

  iss >> command;  // Получаем команду

  if (command == "PUSH") {
    iss >> value;       // Получаем значение
    stack.push(value);  // Добавляем значение в стек
    stack.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Добавлено в стек: " << value << "\n";
  } else if (command == "POP") {
    if (stack.GetSize() > 0) {
      stack.pop();  // Удаляем элемент из стека
      stack.writeToFile(fileName);  // Записываем обратно в файл
      cout << "Удален из стека элемент.\n";
    } else {
      cerr << "Стек пуст, ничего не удалено.\n";
    }
  } else if (command == "PRINT") {
    stack.print();  // Печатаем элементы стека
  } else {
    cerr << "Неверная команда для стека: " << command << "\n";
  }
}

void DBMS::handleTree(const string& fileName, const string& query) {
  CompleteBinaryTree tree(100);  // Примерный размер дерева
  tree.fillTreeFromFile(fileName);  // Заполнение дерева из файла

  istringstream iss(query);
  string command;
  int value;

  iss >> command;  // Получаем команду

  if (command == "INSERT") {
    iss >> value;        // Получаем значение
    tree.insert(value);  // Вставляем значение в дерево
    tree.writeToFile(fileName);  // Записываем обратно в файл
    cout << "Вставлено в дерево: " << value << "\n";
  } else if (command == "SEARCH") {
    iss >> value;  // Получаем значение для поиска
    if (tree.search(value)) {
      cout << "Значение " << value << " найдено в дереве.\n";
    } else {
      cout << "Значение " << value << " не найдено в дереве.\n";
    }
  } else if (command == "PRINT") {
    tree.printLevelOrder();  // Печатаем дерево
  } else if (command == "ISC") {
    if (tree.isComplete()) {
      cout << "Дерево полное.\n";
    } else {
      cout << "Дерево не полное.\n";
    }
  } else {
    cerr << "Неверная команда для дерева: " << command << "\n";
  }
}
