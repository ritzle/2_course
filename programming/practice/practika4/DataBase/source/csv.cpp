#include "../headers/SYBD/csv.hpp"

// Конструктор по умолчанию
CSV::CSV() : csvName(""), columns(), line() {}

// Конструктор с параметрами
CSV::CSV(const string name, const Array<string> cols)
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

json CSV::serialize() const {
  json j;
  j["csvName"] = csvName;

  // Сериализация columns (массив строк)
  j["columns"] = columns.to_stringJson();

  // Сериализация line как массива строк
  json lineArray = json::array();
  for (size_t i = 0; i < line.getSize(); i++) {
    lineArray.push_back(
        line[i].to_stringJson());  // Каждую строку сериализуем отдельно
  }
  j["line"] = lineArray;

  return j;
}

// // десериализация
// CSV CSV::deserialize(const json& j) {
//   CSV csv;

//   // Десериализация csvName
//   if (j.contains("csvName") && j["csvName"].is_string()) {
//     csv.csvName = j["csvName"];
//   }

//   // Десериализация columns
//   if (j.contains("columns") && j["columns"].is_array()) {
//     Array<std::string> deserializedColumns;
//     for (const auto& col : j["columns"]) {
//       if (col.is_string()) {
//         deserializedColumns.push_back(col.get<std::string>());
//       }
//     }
//     csv.columns = deserializedColumns;
//   }

//   // Десериализация line как массива строк
//   if (j.contains("line") && j["line"].is_array()) {
//     Array<Array<std::string>> deserializedLines;

//     // Перебираем каждую строку в массиве "line"
//     for (const auto& lineJson : j["line"]) {
//       if (lineJson.is_string()) {
//         std::string currentLine = lineJson.get<std::string>();

//         // Разбиваем строку на столбцы, разделенные запятой
//         std::istringstream lineStream(currentLine);
//         std::string columnValue;

//         while (std::getline(lineStream, columnValue, ',')) {
//           Array<std::string> deserializedRow;
//           deserializedRow.from_stringJson(columnValue);

//           deserializedLines.push_back(deserializedRow);
//           deserializedRow.clear();
//         }
//       }
//     }

//     csv.line = deserializedLines;  // Добавляем все строки
//   }

//   return csv;
// }

CSV CSV::deserialize(const json& j) {
  CSV csv;

  // Десериализация csvName
  if (j.contains("csvName") && j["csvName"].is_string()) {
    csv.csvName = j["csvName"];
  }

  // Десериализация columns
  if (j.contains("columns") && j["columns"].is_string()) {
    std::string columnsString = j["columns"].get<std::string>();
    std::string columnValue;
    Array<std::string> deserializedColumns;
    deserializedColumns.from_stringJson(columnsString);

    // while (columnsStream >> columnValue) {  // Разделение по пробелам
    //   deserializedColumns.push_back(columnValue);
    // }
    csv.columns = deserializedColumns;
  }

  // Десериализация line
  if (j.contains("line") && j["line"].is_array()) {
    Array<Array<std::string>> deserializedLines;

    for (const auto& lineJson : j["line"]) {
      if (lineJson.is_string()) {
        std::string lineString = lineJson.get<std::string>();
        // cout << "--" << lineString << endl;
        Array<std::string> columnValue;

        columnValue.from_stringJson(lineString);

        deserializedLines.push_back(columnValue);
      }
    }

    csv.line = deserializedLines;  // Добавляем все строки
  }

  return csv;
}

// Функция для удаления лишних пробелов (используется для очистки строки)
std::string trim(const std::string& str) {
  size_t first = str.find_first_not_of(" \t\n\r");
  size_t last = str.find_last_not_of(" \t\n\r");
  return (first == std::string::npos || last == std::string::npos)
             ? ""
             : str.substr(first, last - first + 1);
}