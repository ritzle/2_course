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

  // получаем непосредственно список значений для добавления
  Array<string> parsedValues = parseValues(valuesList);

  db.insertIntoTable(tableName, parsedValues);
}

// Обработка команды SELECT
void SQLParser::handleSelect(istringstream& stream) {}

// Обработка команды DELETE
void SQLParser::handleDelete(istringstream& stream) {}

// Обработка команды WHERE
void SQLParser::handleWhere(istringstream& stream) {
  string whereKeyword;
  stream >> whereKeyword;

  if (whereKeyword != "WHERE") {
    throw runtime_error("Ожидалось 'WHERE', но получено: " + whereKeyword);
  }

  string condition;
  getline(stream, condition);

  condition.erase(0, condition.find_first_not_of(" \t\n\r\f\v"));
  condition.erase(condition.find_last_not_of(" \t\n\r\f\v") + 1);

  // Обработка условий с использованием AND и OR
  Array<Array<string>> parsedConditions = parseConditions(condition);

  // TODO подумать так как where не используется сам по себе то в bd он не нужен
  // TODO и будет вызываться в другий запросах
  // db.applyWhereConditions(tableName,parsedConditions);
}

// FIXME доработать
Array<Array<string>> SQLParser::parseConditions(string& condition) {
  Array<Array<string>> parsedConditions;  // Внешний массив для условий OR
  Array<string> currentAndConditions;  // Временный вектор для условий AND

  // Убираем лишние пробелы в начале и конце
  condition.erase(0, condition.find_first_not_of(" \t\n\r\f\v"));
  condition.erase(condition.find_last_not_of(" \t\n\r\f\v") + 1);

  size_t pos = 0;

  while ((pos = condition.find("OR")) != string::npos) {
    string orCondition = condition.substr(0, pos);
    condition.erase(0, pos + 2);  // Удаляем "OR"

    // Обрабатываем условия AND в текущем фрагменте
    size_t andPos = 0;
    while ((andPos = orCondition.find("AND")) != string::npos) {
      string andCondition = orCondition.substr(0, andPos);
      orCondition.erase(0, andPos + 3);  // Удаляем "AND"

      andCondition.erase(0, andCondition.find_first_not_of(" \t\n\r\f\v"));
      andCondition.erase(andCondition.find_last_not_of(" \t\n\r\f\v") + 1);

      currentAndConditions.push_back(andCondition);
    }

    // Добавляем остаток условия AND
    if (!orCondition.empty()) {
      orCondition.erase(
          0, orCondition.find_first_not_of(" \t\n\r\f\v"));  // Убираем пробелы
      orCondition.erase(orCondition.find_last_not_of(" \t\n\r\f\v") + 1);
      currentAndConditions.push_back(orCondition);
    }

    // Добавляем текущую группу AND в массив OR
    if (!currentAndConditions.empty()) {
      parsedConditions.push_back(currentAndConditions);
      currentAndConditions.clear();  // Очищаем текущую группу
    }
  }

  // Обработка последнего фрагмента после последнего OR
  if (!condition.empty()) {
    // Убираем лишние пробелы
    condition.erase(0, condition.find_first_not_of(" \t\n\r\f\v"));
    condition.erase(condition.find_last_not_of(" \t\n\r\f\v") + 1);

    // Обрабатываем условия AND в последнем фрагменте
    size_t andPos = 0;
    while ((andPos = condition.find("AND")) != string::npos) {
      string andCondition = condition.substr(0, andPos);
      condition.erase(0, andPos + 3);  // Удаляем "AND"

      // Убираем лишние пробелы
      andCondition.erase(0, andCondition.find_first_not_of(" \t\n\r\f\v"));
      andCondition.erase(andCondition.find_last_not_of(" \t\n\r\f\v") + 1);

      currentAndConditions.push_back(andCondition);
    }

    // Добавляем остаток условия AND
    if (!condition.empty()) {
      condition.erase(
          0, condition.find_first_not_of(" \t\n\r\f\v"));  // Убираем пробелы
      condition.erase(condition.find_last_not_of(" \t\n\r\f\v") + 1);
      currentAndConditions.push_back(condition);
    }

    // Если остались условия в текущей группе AND, добавляем их
    if (!currentAndConditions.empty()) {
      parsedConditions.push_back(currentAndConditions);
    }
  }

  return parsedConditions;  // Возвращаем структуру условий
}

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
