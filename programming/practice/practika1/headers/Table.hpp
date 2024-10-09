#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>

#include "../headers/array.hpp"

using namespace std;

class Table {
 public:
  string tableName;
  string pathTable;
  Array<string> columns;
  Array<Array<string>> line;
  int tuplesLimit;
  int countCSVFile;
  int pk_sequence;
  int lock;  // 0-фаил открыт 1-фаил закрыт

  // Конструктор по умолчанию
  Table();

  // Конструктор с параметрами
  Table(const string& name, const Array<string>& cols);

  // Конструктор копирования
  Table(const Table& other);

  // Оператор присваивания
  Table& operator=(const Table& other);
};

#include "../source/Table.cpp"

#endif  // TABLE_H