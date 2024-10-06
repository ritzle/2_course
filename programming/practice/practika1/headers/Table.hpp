#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>

#include "../headers/array.hpp"

using namespace std;

class Table {
 public:
  string tableName;
  Array<string> columns;
  int tuplesLimit;

  // Конструктор по умолчанию
  Table();

  // Конструктор с параметрами
  Table(const string& name, const Array<string>& cols);

  // Конструктор копирования
  Table(const Table& other);

  // Оператор присваивания
  Table& operator=(const Table& other);

  // Метод для печати информации о таблице
  void print();
};

#include "../source/Table.cpp"

#endif  // TABLE_H
