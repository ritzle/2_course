#include "../headers/csv.hpp"

// Конструктор по умолчанию
CSV::CSV() : csvName(""), columns(), line() {}

// Конструктор с параметрами
CSV::CSV(const string& name, const Array<string>& cols)
    : csvName(name), columns(cols), line() {}

CSV::CSV(const string& name) : csvName(name), columns(), line() {}

int CSV::countLine() { return line.getSize(); }

string CSV::getColumnValue(const string& columnName, int rowIndex) const {
  // Проверим, что индекс строки корректный
  if (rowIndex < 0 || rowIndex >= line.getSize()) {
    cerr << "Ошибка: индекс строки вне допустимого диапазона" << endl;
    return "";
  }

  // Найдем индекс колонки по её имени
  int columnIndex = -1;
  for (int i = 0; i < columns.getSize(); ++i) {
    if (columns[i] == columnName) {
      columnIndex = i;
      break;
    }
  }

  // Проверим, что колонка найдена
  if (columnIndex == -1) {
    cerr << "Ошибка: колонка '" << columnName << "' не найдена" << endl;
    return "";
  }

  // Возвращаем значение из строки данных по индексу найденной колонки
  return line[rowIndex][columnIndex];
}
