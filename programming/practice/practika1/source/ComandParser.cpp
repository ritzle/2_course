#include "../headers/ComandParser.hpp"

SQLParser::SQLParser(DB& database) : db(database) {}

// Основная функция для обработки SQL-запросов
void SQLParser::parse(const string& query) {
  istringstream stream(query);
  string command;
  stream >> command;

  if (command == "INSERT") {
    handleInsert(stream);
  } else if (command == "SELECT") {
    handleSelect(stream);
  } else if (command == "DELETE") {
    handleDelete(stream);
  } else if (command == "WHERE") {
    handleWhere(stream);
  } else {
    throw runtime_error("Неизвестная команда: " + command);
  }
}

// Обработка команды INSERT
void SQLParser::handleInsert(istringstream& stream) {
  string into;
  stream >> into;

  if (into != "INTO") {
    throw runtime_error("Ожидалось 'INTO', но получено: " + into);
  }

  string tableName;
  stream >> tableName;

  string values;
  stream >> values;  // Ожидаем "VALUES"

  if (values != "VALUES") {
    throw runtime_error("Ожидалось 'VALUES', но получено: " + values);
  }

  string valuesList;
  getline(stream, valuesList, ')');

  Array<string> parsedValues = parseValues(valuesList);

  db.insertIntoTable(tableName, parsedValues);
}

// Обработка команды SELECT
void SQLParser::handleSelect(istringstream& stream) {}

// Обработка команды DELETE
void SQLParser::handleDelete(istringstream& stream) {}

void SQLParser::handleWhere(istringstream& stream) {}

Array<string> SQLParser::parseValues(const string& valuesList) {
  Array<string> values;
  string cleanedValues = valuesList;

  // Убираем все пробелы в начале и конце строки
  cleanedValues.erase(0, cleanedValues.find_first_not_of(" \t\n\r\f\v"));
  cleanedValues.erase(cleanedValues.find_last_not_of(" \t\n\r\f\v") + 1);

  // Убираем круглые скобки
  if (!cleanedValues.empty() && cleanedValues.front() == '(') {
    cleanedValues.erase(0, 1);
  }
  if (!cleanedValues.empty() && cleanedValues.back() == ')') {
    cleanedValues.erase(cleanedValues.size() - 1);
  }

  size_t start = 0;
  size_t end = 0;

  // Проходим по строке и извлекаем значения между кавычками
  while ((start = cleanedValues.find('\'', end)) != string::npos) {
    end = cleanedValues.find('\'', start + 1);

    if (end != string::npos) {
      string value = cleanedValues.substr(start + 1, end - start - 1);
      values.push_back(value);
      end++;
    }
  }

  return values;
}
