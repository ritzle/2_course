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
  string condition;
  getline(stream, condition);

  condition.erase(0, condition.find_first_not_of(" \t\n\r\f\v"));
  condition.erase(condition.find_last_not_of(" \t\n\r\f\v") + 1);

  // Обработка условий с использованием AND и OR
  Array<Array<string>> parsedConditions = parseConditions(condition);

  db.applyWhereConditions(parsedConditions);
}

Array<Array<string>> SQLParser::parseConditions(string& query) {
  Array<Array<string>> result;

  // Лямбда-функция для разбора части строки с условиями через 'AND'
  auto parseAndConditions = [](const string& subquery) -> Array<string> {
    Array<string> conditions;
    size_t start = 0;
    size_t andPos = subquery.find(" AND ");

    while (andPos != string::npos) {
      // Вырезаем условие между двумя 'AND'
      string condition = subquery.substr(start, andPos - start);
      conditions.push_back(condition);  // Добавляем условие

      // Обновляем индексы для следующего поиска
      start = andPos + 5;  // Пропускаем " AND "
      andPos = subquery.find(" AND ", start);
    }

    // Добавляем последнюю часть, которая идёт после последнего 'AND'
    if (start < subquery.size()) {
      string condition = subquery.substr(start);
      conditions.push_back(condition);
    }

    return conditions;
  };

  size_t orPos = query.find(" OR ");  // Ищем первое вхождение 'OR'

  if (orPos != string::npos) {
    // Обрабатываем часть до 'OR'
    string leftPart = query.substr(0, orPos);
    result.push_back(
        parseAndConditions(leftPart));  // Разбираем и добавляем левую часть

    // Обрабатываем часть после 'OR'
    string rightPart = query.substr(orPos + 4);  // Обрезаем строку после 'OR'
    result.push_back(
        parseAndConditions(rightPart));  // Разбираем и добавляем правую часть
  } else {
    // Если 'OR' не найден, просто разбираем всю строку
    result.push_back(parseAndConditions(query));
  }

  return result;
}

// в insert используется
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
