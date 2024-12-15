#ifndef TABLE_H
#define TABLE_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../../lib/json.hpp"
#include "../array.hpp"
#include "csv.hpp"

using namespace std;

using json = nlohmann::json;

class Table {
 public:
  string tableName;
  string pathTable;
  Array<CSV> csv;
  Array<string> baseColumns;
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

  void countCSVFiles();  // Подсчитывает количество CSV-файлов
  void readLockFile();  // Читает файл блокировки
  void readPKSequenceFile();  // Читает файл последовательности pk_sequence
  int counterAllLine();

  // Метод для получения массива значений определенной колонки
  Array<string> getColumnValues(const string& columnName);

  json serialize() const;
  Table deserialize(const json& j);

 private:
  // Метод для поиска индекса колонки по имени
  int findColumnIndex(const CSV& csv, const string& columnName) const;
};

#include "../../source/Table.cpp"

#endif  // TABLE_H
