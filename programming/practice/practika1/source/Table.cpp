#include "../headers/Table.hpp"

// Конструктор по умолчанию
Table::Table() : tableName(""), columns(Array<string>(2)), tuplesLimit(100) {}

// Конструктор с параметрами
Table::Table(const string& name, const Array<string>& cols)
    : tableName(name), columns(cols) {}

// Конструктор копирования
Table::Table(const Table& other)
    : tableName(other.tableName),
      columns(other.columns),
      tuplesLimit(other.tuplesLimit) {}

// Оператор присваивания
Table& Table::operator=(const Table& other) {
  if (this != &other) {  // Проверка на самоприсваивание
    tableName = other.tableName;
    columns = other.columns;  // Предполагается, что оператор присваивания для
                              // Array корректно реализован
    tuplesLimit = other.tuplesLimit;
  }
  return *this;
}

// Метод печати
void Table::print() {
  std::cout << "Table: " << tableName << std::endl;
  std::cout << "Columns: ";
  for (auto& col : columns) {
    std::cout << col << " ";
  }
  std::cout << std::endl;
}
