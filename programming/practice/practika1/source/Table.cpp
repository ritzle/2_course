#include "../headers/Table.hpp"

// Конструктор по умолчанию
Table::Table()
    : tableName(""),
      columns(),
      pk_sequence(0),
      pathTable(),
      countCSVFile(1),
      tuplesLimit(100) {}  // Используйте конструктор по умолчанию для Array

// Конструктор с параметрами
Table::Table(const string& name, const Array<string>& cols)
    : tableName(name), countCSVFile(1), columns(cols) {}

// Конструктор копирования
Table::Table(const Table& other)
    : tableName(other.tableName),
      columns(other.columns),
      pathTable(other.pathTable),
      pk_sequence(other.pk_sequence),
      countCSVFile(other.countCSVFile),
      tuplesLimit(other.tuplesLimit) {}

// Оператор присваивания
Table& Table::operator=(const Table& other) {
  if (this != &other) {  // Проверка на самоприсваивание
    tableName = other.tableName;
    columns = other.columns;
    pathTable = other.pathTable;
    pk_sequence = other.pk_sequence;
    tuplesLimit = other.tuplesLimit;
    countCSVFile = other.countCSVFile;
  }
  return *this;
}
