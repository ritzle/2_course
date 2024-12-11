#include "../headers/SYBD/SYBD.hpp"

#include <filesystem>  // Для проверки существования директории
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;
using namespace std;

DB::DB(){};

// чтение конфигурации
// чтение конфигурации
void DB::readingConfiguration(string PathSchema) {
  ifstream file(PathSchema);

  if (!file.is_open()) {
    throw runtime_error("Не удалось открыть файл: " + PathSchema);
  }

  // Читаем JSON файл и парсим его в объект json
  json JFile;
  file >> JFile;

  if (JFile.contains("name") && JFile.contains("tuples_limit") &&
      JFile.contains("structure")) {
    schemaName = JFile["name"];
    tuplesLimit = JFile["tuples_limit"];

    for (auto& JTable : JFile["structure"].items()) {
      Table table;
      table.tableName = JTable.key();
      table.tuplesLimit = this->tuplesLimit;

      // Добавляем первичный ключ в baseColumns
      table.baseColumns.push_back(table.tableName + "_pk");

      // Читаем колонки и добавляем их в baseColumns
      for (auto& col : JTable.value()) {
        if (col.is_string()) {
          table.baseColumns.push_back(col.get<string>());
        } else {
          throw runtime_error("Колонка должна быть строкой");
        }
      }

      // Создаем CSV объект и копируем baseColumns в columns
      CSV csv;
      csv.columns = table.baseColumns;

      table.csv.push_back(csv);  // Добавляем CSV объект в таблицу
      structure.push_back(table);
    }
  } else {
    throw runtime_error("Отсутствуют необходимые ключи в JSON файле");
  }

  // Проверяем, существует ли директория схемы
  string schemaPath = "../" + schemaName;
  if (filesystem::exists(schemaPath)) {
    // Если директория существует, загружаем данные существующей схемы
    cout << "Схема " << schemaName
         << " уже существует"
            "";
    // loadExistingSchemaData();
  } else {
    createDirectoriesAndFiles();
  }

  file.close();
}

void DB::loadExistingSchemaData(Array<string>& tableNames) {
  for (auto& table : structure) {
    // Проверяем, нужно ли загружать данную таблицу
    if (!tableNames.contains(table.tableName)) {
      continue;  // Пропускаем таблицы, не указанные в tableNames
    }

    table.pathTable = fs::path("..") / schemaName / table.tableName;

    // 1. Считаем количество CSV файлов для таблицы
    table.countCSVFiles();

    // 2. Читаем информацию из файла блокировки и pk_sequence
    table.readLockFile();
    table.readPKSequenceFile();

    // 3. Инициализируем нужное количество CSV объектов
    while (table.csv.getSize() < table.countCSVFile) {
      string nameCSV = to_string(table.csv.getSize() + 1) + ".csv";
      table.csv.push_back(CSV(nameCSV));
    }

    // FIXME Костыль
    table.csv[0].csvName = "1.csv";

    // 4. Загружаем данные из всех CSV файлов
    for (int i = 1; i <= table.countCSVFile; ++i) {
      string csvFilePath = fs::path(table.pathTable) / (to_string(i) + ".csv");
      ifstream csvFile(csvFilePath);

      if (!csvFile.is_open()) {
        throw runtime_error("Не удалось открыть файл таблицы: " + csvFilePath);
      }

      string line;
      bool isFirstLine = true;  // Флаг для отслеживания первой строки
      while (getline(csvFile, line)) {
        if (line.empty()) continue;  // Пропускаем пустые строки

        Array<string> row = parseCSVLine(line);

        if (isFirstLine) {
          // Сохраняем первую строку в columns
          table.csv[i - 1].columns = row;
          isFirstLine = false;  // После обработки первой строки
        } else {
          // Все остальные строки добавляем в line
          table.csv[i - 1].line.push_back(row);
        }
      }

      csvFile.close();  // Закрываем файл после обработки
    }

    cout << "Таблица " << table.tableName
         << " загружена успешно. Количество строк: "
         << to_string(table.counterAllLine()) << endl;
  }
}

void DB::loadExistingSchemaData(const Array<string>& tableNames) {
  for (auto& table : structure) {
    // Проверяем, нужно ли загружать данную таблицу
    if (!tableNames.contains(table.tableName)) {
      continue;  // Пропускаем таблицы, не указанные в tableNames
    }

    table.pathTable = fs::path("..") / schemaName / table.tableName;

    // 1. Считаем количество CSV файлов для таблицы
    table.countCSVFiles();

    // 2. Читаем информацию из файла блокировки и pk_sequence
    table.readLockFile();
    table.readPKSequenceFile();

    // 3. Инициализируем нужное количество CSV объектов
    while (table.csv.getSize() < table.countCSVFile) {
      string nameCSV = to_string(table.csv.getSize() + 1) + ".csv";
      table.csv.push_back(CSV(nameCSV));
    }

    // FIXME Костыль
    table.csv[0].csvName = "1.csv";

    // 4. Загружаем данные из всех CSV файлов
    for (int i = 1; i <= table.countCSVFile; ++i) {
      string csvFilePath = fs::path(table.pathTable) / (to_string(i) + ".csv");
      ifstream csvFile(csvFilePath);

      if (!csvFile.is_open()) {
        throw runtime_error("Не удалось открыть файл таблицы: " + csvFilePath);
      }

      string line;
      bool isFirstLine = true;  // Флаг для отслеживания первой строки
      while (getline(csvFile, line)) {
        if (line.empty()) continue;  // Пропускаем пустые строки

        Array<string> row = parseCSVLine(line);

        if (isFirstLine) {
          // Сохраняем первую строку в columns
          table.csv[i - 1].columns = row;
          isFirstLine = false;  // После обработки первой строки
        } else {
          // Все остальные строки добавляем в line
          table.csv[i - 1].line.push_back(row);
        }
      }

      csvFile.close();  // Закрываем файл после обработки
    }

    cout << "Таблица " << table.tableName
         << " загружена успешно. Количество строк: "
         << to_string(table.counterAllLine()) << endl;
  }
}

void DB::loadExistingSchemaData(string& tableName) {
  for (auto& table : structure) {
    // Проверяем, нужно ли загружать данную таблицу
    if (tableName != table.tableName) {
      continue;  // Пропускаем таблицы, не указанные в tableNames
    }

    table.pathTable = fs::path("..") / schemaName / table.tableName;

    // 1. Считаем количество CSV файлов для таблицы
    table.countCSVFiles();

    // 2. Читаем информацию из файла блокировки и pk_sequence
    table.readLockFile();
    table.readPKSequenceFile();

    // 3. Инициализируем нужное количество CSV объектов
    // TODOg++

    if (table.countCSVFile == 0 && table.csv.getSize() == 0) {
      string nameCSV = to_string(table.csv.getSize() + 1) + ".csv";
      table.csv.push_back(CSV(nameCSV));
      table.csv[0].columns = table.baseColumns.copy();
    } else {
      while (table.csv.getSize() < table.countCSVFile) {
        string nameCSV = to_string(table.csv.getSize() + 1) + ".csv";
        table.csv.push_back(CSV(nameCSV));
      }
    }

    // FIXME Костыль
    table.csv[0].csvName = "1.csv";

    // 4. Загружаем данные из всех CSV файлов
    for (int i = 1; i <= table.countCSVFile; ++i) {
      string csvFilePath = fs::path(table.pathTable) / (to_string(i) + ".csv");
      ifstream csvFile(csvFilePath);

      if (!csvFile.is_open()) {
        throw runtime_error("Не удалось открыть файл таблицы: " + csvFilePath);
      }

      string line;
      bool isFirstLine = true;  // Флаг для отслеживания первой строки
      while (getline(csvFile, line)) {
        if (line.empty()) continue;  // Пропускаем пустые строки

        Array<string> row = parseCSVLine(line);

        if (isFirstLine) {
          // Сохраняем первую строку в columns
          table.csv[i - 1].columns = row;
          isFirstLine = false;  // После обработки первой строки
        } else {
          // Все остальные строки добавляем в line
          table.csv[i - 1].line.push_back(row);
        }
      }

      csvFile.close();  // Закрываем файл после обработки
    }

    cout << "Таблица " << table.tableName
         << " загружена успешно. Количество строк: "
         << to_string(table.counterAllLine()) << endl;
  }
}

void DB::unloadSchemaData(Array<string>& tableNames) {
  for (auto& table : structure) {
    // Проверяем, нужно ли очищать данную таблицу
    if (!tableNames.contains(table.tableName)) {
      continue;  // Пропускаем таблицы, не указанные в tableNames
    }

    // 1. Очистка данных CSV файлов
    for (auto& csv : table.csv) {
      // csv.columns.clear();  // Очищаем столбцы
      csv.line.clear();  // Очищаем строки
    }

    // 2. Очистка информации о файлах
    table.csv.clear();  // Очищаем массив CSV объектов
    table.countCSVFile = 0;  // Сбрасываем количество CSV файлов
    table.pathTable.clear();  // Очищаем путь к таблице
    table.pk_sequence = 0;  // Сбрасываем значение pk_sequence
    table.lock = 0;  // Сбрасываем флаг наличия блокировки

    cout << "Таблица " << table.tableName << " успешно выгружена из памяти."
         << endl;
  }
}

void DB::unloadSchemaData(const Array<string>& tableNames) {
  for (auto& table : structure) {
    // Проверяем, нужно ли очищать данную таблицу
    if (!tableNames.contains(table.tableName)) {
      continue;  // Пропускаем таблицы, не указанные в tableNames
    }

    // 1. Очистка данных CSV файлов
    for (auto& csv : table.csv) {
      // csv.columns.clear();  // Очищаем столбцы
      csv.line.clear();  // Очищаем строки
    }

    // 2. Очистка информации о файлах
    table.csv.clear();  // Очищаем массив CSV объектов
    table.countCSVFile = 0;  // Сбрасываем количество CSV файлов
    table.pathTable.clear();  // Очищаем путь к таблице
    table.pk_sequence = 0;  // Сбрасываем значение pk_sequence
    table.lock = 0;  // Сбрасываем флаг наличия блокировки

    cout << "Таблица " << table.tableName << " успешно выгружена из памяти."
         << endl;
  }
}

void DB::unloadSchemaData(string& tableNames) {
  for (auto& table : structure) {
    // Проверяем, нужно ли очищать данную таблицу
    if (tableNames != table.tableName) {
      continue;  // Пропускаем таблицы, не указанные в tableNames
    }

    for (auto& csv : table.csv) {
      // csv.columns.clear();  // Очищаем столбцы
      csv.line.clear();  // Очищаем строки
    }

    // 2. Очистка информации о файлах
    table.csv.clear();  // Очищаем массив CSV объектов
    table.countCSVFile = 0;  // Сбрасываем количество CSV файлов
    table.pathTable.clear();  // Очищаем путь к таблице
    table.pk_sequence = 0;  // Сбрасываем значение pk_sequence
    table.lock = 0;  // Сбрасываем флаг наличия блокировки

    cout << "Таблица " << table.tableName << " успешно выгружена из памяти."
         << endl;
  }
}

Array<string> DB::parseCSVLine(const string& line) {
  Array<string> parsedRow;
  stringstream ss(line);
  string value;

  while (getline(ss, value, ',')) {
    value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
    value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
    parsedRow.push_back(value);
  }

  return parsedRow;
}

void DB::createDirectoriesAndFiles() {
  // Задаем путь к папке practica1, внутри которой создадим папку схемы
  fs::path practicaDir = fs::path("..") / schemaName;

  // Создаем директорию для схемы, если ее еще нет
  if (!fs::exists(practicaDir)) {
    fs::create_directory(practicaDir);
  }

  // Перебираем все таблицы в структуре
  for (size_t i = 0; i < structure.getSize(); ++i) {
    Table& table = structure[i];

    // Создаем директорию для таблицы внутри папки схемы
    fs::path tableDir = practicaDir / table.tableName;
    if (!fs::exists(tableDir)) {
      fs::create_directory(tableDir);
    }

    table.pathTable = tableDir;

    // Создаем файл CSV для таблицы
    fs::path csvFile = tableDir / "1.csv";
    ofstream file(csvFile);
    if (file.is_open()) {
      for (size_t j = 0; j < table.csv[0].columns.getSize(); ++j) {
        file << table.csv[0].columns[j];
        if (j < table.csv[0].columns.getSize() - 1) {
          file << ",";
        }
      }
      file << endl;
      file.close();
    } else {
      cerr << "Ошибка при создании файла: " << csvFile << endl;
    }

    // sequence
    fs::path pkSeqenceFile = tableDir / (table.tableName + "_pk_seqence");
    if (!fs::exists(pkSeqenceFile)) {
      ofstream file(pkSeqenceFile);
      if (file.is_open()) {
        file << table.pk_sequence;
        file.close();
      } else {
        cerr << "Ошибка при создании файла: " << table.tableName + "_pk_seqence"
             << endl;
      }
    }

    // lock
    fs::path lockFile = tableDir / (table.tableName + "_lock");
    if (!fs::exists(lockFile)) {
      ofstream file(lockFile);
      if (file.is_open()) {
        file << table.lock;
        file.close();
      } else {
        cerr << "Ошибка при создании файла: " << table.tableName + "_lock"
             << endl;
      }
    }
  }
}

void DB::insertIntoTable(string TableName, Array<string> arrValues) {
  loadExistingSchemaData(TableName);

  Table& currentTable = searchTable(TableName);

  currentTable.readLockFile();

  // Ожидание разблокировки таблицы
  while (currentTable.lock == 1) {
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "жду разблокировки файла";
    currentTable.readLockFile();
  }

  // Блокируем таблицу для работы
  currentTable.lock = 1;
  updateLock(currentTable);

  ++currentTable.pk_sequence;
  arrValues.insert_beginning(
      to_string(currentTable.pk_sequence));  // уникальное число

  // Проверяем, достиг ли лимит строк в текущем CSV файле
  if (currentTable.csv.back().line.getSize() >= tuplesLimit) {
    if (currentTable.csv.getSize() == 0) {
      CSV newCsv("1.csv", currentTable.csv.back().columns);
      currentTable.csv.push_back(newCsv);
      currentTable.countCSVFile++;  // Обновляем количество CSV файлов
    }

    // Создаем новый CSV файл
    string newCsvName = to_string(currentTable.csv.getSize() + 1) + ".csv";
    CSV newCsv(newCsvName, currentTable.csv.back().columns.copy());
    currentTable.csv.push_back(newCsv);
    currentTable.countCSVFile++;  // Обновляем количество CSV файлов
  }

  // Добавляем строку в последний CSV

  if (arrValues.getSize() == currentTable.csv[0].columns.getSize()) {
    currentTable.csv.back().line.push_back(arrValues);
  } else if (arrValues.getSize() < currentTable.csv[0].columns.getSize()) {
    while (arrValues.getSize() != currentTable.csv[0].columns.getSize()) {
      arrValues.push_back("null");
    }
    currentTable.csv.back().line.push_back(arrValues);
  } else {
    cerr << "---аргументов больше чем колонок----\n";
  }

  // Обновляем только один раз после завершения вставки
  updateCSVFile(currentTable);

  // Разблокируем таблицу
  currentTable.lock = 0;
  updateLock(currentTable);
  updatePkSeqence(currentTable);

  unloadSchemaData(TableName);
}

void DB::applySelect(const Array<string>& tableNames,
                     const Array<string>& tableColumns) {
  loadExistingSchemaData(tableNames);

  // Проверка наличия таблиц и колонок
  if (tableNames.getSize() == 0 || tableColumns.getSize() == 0) {
    cerr << "Ошибка: отсутствуют данные" << endl;
    return;
  }

  // Считываем таблицы
  Array<Table> involvedTables;
  for (const string& tableName : tableNames) {
    try {
      Table table = searchTable(tableName);

      if (table.csv.getSize() == 0) {
        cerr << "Ошибка: таблица " << tableName << " пуста." << endl;
        return;
      }
      involvedTables.push_back(table);
    } catch (const runtime_error& e) {
      cerr << "Ошибка: не удалось найти таблицу " << tableName << ". "
           << e.what() << endl;
      return;
    }
  }

  Array<string> crossJoinResult;

  // Проходим по колонкам и заполняем кросс-соединение
  for (const string& columnString : tableColumns) {
    size_t dotPos = columnString.find('.');
    if (dotPos == string::npos) {
      cerr << "Ошибка: неверный формат имени колонки " << columnString << endl;
      return;
    }

    string tableName = columnString.substr(0, dotPos);
    string columnName = columnString.substr(dotPos + 1);

    // Получаем значения столбца из таблицы
    Table table = searchTable(tableName);
    Array<string> columnValues = table.getColumnValues(columnName);

    if (crossJoinResult.getSize() == 0) {
      crossJoinResult = columnValues;
    } else {
      crossJoinResult = crossJoin(crossJoinResult, columnValues);
    }
  }

  string name = "SELECT_";
  for (auto& col : tableColumns) {
    name += col + "_";
    cout << col << " ";
  }
  rewriteFil(name, tableColumns, crossJoinResult);
  unloadSchemaData(tableNames);
}

void DB::applyWhereConditions(const Array<string>& tableNames,
                              const Array<string>& tableColumns,
                              const Array<Array<string>>& conditional) {
  loadExistingSchemaData(tableNames);
  if (conditional.getSize() == 0 || conditional[0].getSize() == 0) {
    cerr << "Ошибка: Условие не указано" << endl;
    return;
  }

  // массив строк для файла фильтрации
  Array<Array<string>> rowFiltration;

  processTableWithConditions(tableNames, conditional,
                             [&](const Array<string>& row) {
                               rowFiltration.insert_beginning(row);
                               row.print();
                               cout << endl;
                             });

  string name = "WHERE_";
  for (auto& col : tableColumns) {
    name += col + "_";
    cout << col << " ";
  }
  rewriteFil(name, rowFiltration);
  unloadSchemaData(tableNames);
}

void DB::applyDeleteConditions(const Array<string>& tableNames,
                               const Array<Array<string>>& conditional) {
  loadExistingSchemaData(tableNames);
  if (conditional.getSize() == 0 || conditional[0].getSize() == 0) {
    cerr << "Ошибка: Условие не указано" << endl;
    return;
  }

  // Проверка и обработка условий для каждой таблицы
  processTableWithConditions(tableNames, conditional,
                             [&](CSV& currentCSV, int indexToDelete) {
                               currentCSV.line.erase(indexToDelete);
                             });
  unloadSchemaData(tableNames);
}

void DB::processTableWithConditions(
    const Array<string>& tableNames, const Array<Array<string>>& conditional,
    function<void(const Array<string>&)> action) {
  // Массив таблиц, участвующих в запросе
  Array<Table> involvedTables;

  // Загружаем все указанные таблицы
  for (int i = 0; i < tableNames.getSize(); ++i) {
    involvedTables.push_back(searchTable(tableNames[i]));
  }

  if (involvedTables.getSize() == 0) {
    throw runtime_error("Ни одной таблицы не найдено");
  }

  // Проходим по каждой таблице
  for (int t = 0; t < involvedTables.getSize(); ++t) {
    Table& currentTable = involvedTables[t];

    if (currentTable.csv.getSize() == 0) {
      throw runtime_error("Таблица " + tableNames[t] + " пуста");
    }

    // Перебираем все CSV-файлы в текущей таблице
    for (int i = 0; i < currentTable.csv.getSize(); ++i) {
      CSV& currentCSV = currentTable.csv[i];

      // Перебираем строки в CSV файла
      for (int j = currentCSV.line.getSize() - 1; j >= 0; --j) {
        Array<string>& row = currentCSV.line[j];

        bool match = false;

        // Перебираем все группы условий (OR)
        for (int k = 0; k < conditional.getSize(); ++k) {
          const Array<string>& conditionGroup = conditional[k];

          // Проверка условий для данной строки с учетом всех таблиц
          if (checkAndConditionsAcrossTables(conditionGroup, row,
                                             currentTable.tableName, i, j)) {
            match = true;

            if (match) {
              action(row);
            }
            break;
          }
        }
      }
    }
  }
}

void DB::processTableWithConditions(const Array<string>& tableNames,
                                    const Array<Array<string>>& conditional,
                                    const function<void(CSV&, int)>& action) {
  // Массив таблиц, участвующих в запросе
  Array<Table> involvedTables;

  // Загружаем все указанные таблицы
  for (int i = 0; i < tableNames.getSize(); ++i) {
    involvedTables.push_back(searchTable(tableNames[i]));
  }

  if (involvedTables.getSize() == 0) {
    throw runtime_error("Ни одной таблицы не найдено");
  }

  // Проходим по каждой таблице
  for (int t = 0; t < involvedTables.getSize(); ++t) {
    Table& currentTable = involvedTables[t];

    currentTable.readLockFile();

    // Ожидание разблокировки таблицы
    while (currentTable.lock == 1) {
      this_thread::sleep_for(chrono::milliseconds(100));
      cout << "жду разблокировки файла";
      currentTable.readLockFile();
    }

    // Блокируем таблицу для работы
    currentTable.lock = 1;
    updateLock(currentTable);

    if (currentTable.csv.getSize() == 0) {
      throw runtime_error("Таблица " + tableNames[t] + " пуста");
    }

    // Перебираем все CSV-файлы в текущей таблице
    for (int i = 0; i < currentTable.csv.getSize(); ++i) {
      CSV& currentCSV = currentTable.csv[i];

      // Перебираем строки в CSV файла
      for (int j = currentCSV.line.getSize() - 1; j >= 0; --j) {
        Array<string>& row = currentCSV.line[j];

        bool match = false;

        // Перебираем все группы условий (OR)
        for (int k = 0; k < conditional.getSize(); ++k) {
          const Array<string>& conditionGroup = conditional[k];

          // Проверка условий для данной строки с учетом всех таблиц
          if (checkAndConditionsAcrossTables(conditionGroup, row,
                                             currentTable.tableName, i, j)) {
            match = true;

            if (match) {
              action(currentCSV, j);
            }
            break;
          }
        }
      }
    }
    currentTable.lock = 0;
    updateLock(currentTable);
    moveLinesBetweenCSVs(currentTable);
    rewriteAllCSVFiles(currentTable);
  }
}

bool DB::checkAndConditionsAcrossTables(const Array<string>& conditionGroup,
                                        const Array<string>& row,
                                        const string& currentTableName,
                                        int currentCSVIndex,
                                        int currentRowIndex) {
  for (int m = 0; m < conditionGroup.getSize(); ++m) {
    string condition = conditionGroup[m];
    size_t eqPos = condition.find('=');

    if (eqPos != string::npos) {
      string leftPart = condition.substr(0, eqPos);  // Левая часть до '='
      string rightPart = condition.substr(eqPos + 1);  // Правая часть после '='

      // Удаляем пробелы
      leftPart.erase(remove(leftPart.begin(), leftPart.end(), ' '),
                     leftPart.end());
      rightPart.erase(remove(rightPart.begin(), rightPart.end(), ' '),
                      rightPart.end());

      // Обработка левой части (имя таблицы и колонка)
      size_t leftDotPos = leftPart.find('.');
      string leftTableName =
          leftPart.substr(0, leftDotPos);  // Имя таблицы слева
      string leftColumn = leftPart.substr(leftDotPos + 1);  // Имя колонки слева

      // Если запросы в одном блоке не отнясятся к одной таблице
      if (leftTableName != currentTableName) {
        return false;  // все не относиться к одной таблице
      }

      // Поиск индекса колонки в текущей таблице
      Table& leftTable = searchTable(leftTableName);
      int leftColumnIndex = findColumnIndex(leftTable.csv[0], leftColumn);
      if (leftColumnIndex == -1) {
        cerr << "Ошибка: Колонка " << leftColumn << " не найдена в таблице "
             << leftTableName << endl;
        return false;
      }

      // Обработка правой части
      bool isRightPartColumn = rightPart.find('.') != string::npos;
      if (isRightPartColumn) {
        // Правая часть - это колонка другой таблицы
        size_t rightDotPos = rightPart.find('.');
        string rightTableName = rightPart.substr(0, rightDotPos);
        string rightColumn = rightPart.substr(rightDotPos + 1);

        // Поиск таблицы для правой части
        Table& rightTable = searchTable(rightTableName);
        int rightColumnIndex = findColumnIndex(rightTable.csv[0], rightColumn);
        if (rightColumnIndex == -1) {
          cerr << "Ошибка: Колонка " << rightColumn << " не найдена в таблице "
               << rightTableName << endl;
          return false;
        }

        // Проверка наличия строки с данным индексом в правой таблице
        if (currentCSVIndex >= rightTable.csv.getSize()) {
          // cerr << "Ошибка: CSV с номером " << currentCSVIndex
          //      << " не найдена в таблице " << rightTableName << endl;
          return false;
        }

        // Проверка наличия строки с данным индексом в правой таблице
        if (currentRowIndex >= rightTable.csv[currentCSVIndex].line.getSize()) {
          // cerr << "Ошибка: Строка с индексом " << currentRowIndex
          //      << " не найдена в таблице " << rightTableName << endl;
          return false;
        }

        // Сравнение значений из обеих таблиц для текущей строки
        string leftValue = row[leftColumnIndex];
        string rightValue = rightTable.csv[currentCSVIndex]
                                .line[currentRowIndex][rightColumnIndex];

        if (leftValue != rightValue) {
          return false;
        }

      } else {
        // Правая часть - это константа
        string value = rightPart;

        // Сравнение значения с колонкой текущей строки
        if (row[leftColumnIndex] != value) {
          return false;
        }
      }
    }
  }
  return true;
}

int DB::findColumnIndex(const CSV& csv, const string& columnName) {
  for (int colIdx = 0; colIdx < csv.columns.getSize(); ++colIdx) {
    if (csv.columns[colIdx] == columnName) {
      return colIdx;
    }
  }
  return -1;  // Если колонка не найдена
}

// Метод для удаления пробелов по краям строки
string DB::trim(const string& str) {
  const char* whitespace = " \t\n\r";
  size_t start = str.find_first_not_of(whitespace);
  size_t end = str.find_last_not_of(whitespace);

  if (start == string::npos || end == string::npos) {
    return "";  // Пустая строка, если только пробелы
  }

  return str.substr(start, end - start + 1);
}

Table& DB::searchTable(const string& TableName) {
  for (auto& table : structure) {
    if (table.tableName == TableName) {
      return table;
    }
  }

  throw runtime_error("Table not found: " + TableName);
}

void DB::updatePkSeqence(Table& table) {
  ofstream out(table.pathTable + "/" + table.tableName + "_pk_seqence");
  if (out.is_open()) {
    out << table.pk_sequence;
  } else {
    cerr << "Ошибка при работе с файлом: " << table.tableName + "_pk_seqence"
         << endl;
  }
}

void DB::updateCSVFile(Table& table) {
  // Путь к файлу
  if (table.countCSVFile == 0) {
    table.countCSVFile++;
  }

  string csvFilePath =
      table.pathTable + "/" + to_string(table.countCSVFile) + ".csv";
  ofstream out(csvFilePath);

  if (out.is_open()) {
    // Записываем заголовки столбцов только один раз
    for (size_t i = 0; i < table.csv.back().columns.getSize(); ++i) {
      out << table.csv.back().columns[i];
      if (i < table.csv.back().columns.getSize() - 1) {
        out << ",";
      }
    }
    out << endl;

    // Записываем строки
    for (const auto& line : table.csv.back().line) {
      for (size_t j = 0; j < line.getSize(); ++j) {
        out << line[j];
        if (j < line.getSize() - 1) {
          out << ",";
        }
      }
      out << endl;
    }

    out.close();
  } else {
    cerr << "Ошибка при открытии файла: " << csvFilePath << endl;
  }
}

void DB::updateLock(Table& table) {
  ofstream out(table.pathTable + "/" + table.tableName + "_lock");
  if (out.is_open()) {
    out << table.lock;
  } else {
    cerr << "Ошибка при работе с файлом: " << table.tableName + "_lock" << endl;
  }
}

void DB::moveLinesBetweenCSVs(Table& table) {
  for (size_t i = 0; i < table.csv.getSize(); ++i) {
    // Проверяем, заполнен ли текущий файл
    while (i < table.csv.getSize() &&
           table.csv[i].line.getSize() < table.tuplesLimit) {
      // Если есть следующий файл, переносим строки
      if (i + 1 < table.csv.getSize() && !table.csv[i + 1].line.empty()) {
        size_t linesToMove = table.tuplesLimit - table.csv[i].line.getSize();

        for (size_t j = 0; j < linesToMove && !table.csv[i + 1].line.empty();
             ++j) {
          table.csv[i].line.push_back(table.csv[i + 1].line.front());
          table.csv[i + 1].line.erase(0);
        }
      } else {
        break;
      }
    }
  }
}

void DB::rewriteAllCSVFiles(Table& table) {
  if (table.csv.getSize() == 0 || table.csv[0].line.getSize() == 0) {
    table.countCSVFiles();

    for (int i = 0; i <= table.countCSVFile; i++) {
      fs::path pathCsvFile = fs::path("..") / schemaName / table.tableName /
                             (to_string(i) + ".csv");
      remove(pathCsvFile);
    }
    table.countCSVFile = 0;

    return;
  }

  for (int i = 0; i < table.csv.getSize(); ++i) {
    rewriteFil(table, i);
  }

  while (table.csv.back().line.getSize() == 0) {
    fs::path pathCsvFile = fs::path("..") / schemaName / table.tableName /
                           table.csv.back().csvName;

    if (fs::remove(pathCsvFile)) {
      table.countCSVFile--;
      table.csv.erase(table.csv.getSize() - 1);
    } else {
      cerr << "Не удалось удалить файл: " << pathCsvFile << endl;
      break;
    }
  }
}

void DB::rewriteFil(Table& table, int numberCsv) {
  // Путь к файлу
  string csvFilePath = table.pathTable + "/" + table.csv[numberCsv].csvName;
  ofstream out(csvFilePath);

  if (out.is_open()) {
    // Записываем заголовки столбцов только один раз из текущего CSV
    const CSV& currentCSV = table.csv[numberCsv];

    // Записываем заголовки столбцов
    for (size_t i = 0; i < currentCSV.columns.getSize(); ++i) {
      out << currentCSV.columns[i];
      if (i < currentCSV.columns.getSize() - 1) {
        out << ",";
      }
    }
    out << endl;

    // Записываем строки
    for (const auto& line : currentCSV.line) {
      for (size_t j = 0; j < line.getSize(); ++j) {
        out << line[j];
        if (j < line.getSize() - 1) {
          out << ",";
        }
      }
      out << endl;
    }

    out.close();
  } else {
    cerr << "Ошибка при открытии файла: " << csvFilePath << endl;
  }
}
void DB::rewriteFil(string& fileName, Array<Array<string>> row) {
  // Путь к файлу

  fs::path FilePath = fs::path("..") / (fileName + ".csv");
  ofstream out(FilePath);

  if (out.is_open()) {
    // Записываем строки
    for (const auto& line : row) {
      for (size_t j = 0; j < line.getSize(); ++j) {
        out << line[j];
        if (j < line.getSize() - 1) {
          out << ",";
        }
      }
      out << endl;
    }

    out.close();
  } else {
    cerr << "Ошибка при открытии файла: " << fileName << endl;
  }
}

void DB::rewriteFil(string& fileName, const Array<string>& columns,
                    const Array<string>& row) {
  // Путь к файлу
  fs::path filePath = fs::path("..") / (fileName + ".csv");
  ofstream out(filePath);

  if (out.is_open()) {
    // Записываем имена столбцов в первую строку
    for (size_t j = 0; j < columns.getSize(); ++j) {
      out << columns[j];
      if (j < columns.getSize() - 1) {
        out << ",";
      }
    }
    out << endl;

    // Записываем данные строки
    for (size_t j = 0; j < row.getSize(); ++j) {
      out << row[j];
      if (j < row.getSize() - 1) {
        out << ",";
      }
      out << endl;
    }

    out.close();
    cout << "Данные успешно записаны в файл: " << filePath << endl;
  } else {
    cerr << "Ошибка при открытии файла для записи: " << fileName << endl;
  }
}

void DB::printInfo() const {
  cout << "Schema Name: " << schemaName << endl;
  cout << "Tuples Limit: " << tuplesLimit << endl;
  cout << "Path to Schema: " << pathSchema << endl;
  cout << "Tables in Structure: " << structure.getSize() << endl;

  cout << endl;

  for (size_t i = 0; i < structure.getSize(); ++i) {
    cout << "Table " << (i + 1) << ": " << structure[i].tableName << endl;
    cout << "count CSV: " << structure[i].countCSVFile << endl;
    cout << "pk_sequence: " << structure[i].pk_sequence << endl;
    cout << "pathTable: " << structure[i].pathTable << endl;

    // Вывод колонок таблицы
    cout << "Columns: ";
    structure[i].csv[0].columns.print();

    // Вывод строк данных
    for (auto& csv : structure[i].csv) {
      for (auto line : csv.line) {
        line.print();
      }
    }

    cout << endl;
  }
  cout << endl;
}

Array<string> DB::crossJoin(Array<string>& first, Array<string>& second) {
  Array<string> result;  // Результирующий массив строк

  // Проходим по каждому элементу из первого массива
  for (size_t i = 0; i < first.getSize(); ++i) {
    const string& firstValue = first[i];

    // Для каждого элемента из первого массива, проходим по элементам второго
    for (size_t j = 0; j < second.getSize(); ++j) {
      const string& secondValue = second[j];

      // Создаем строку, соединяя элементы из первого и второго массивов
      string combinedRow = firstValue + ", " + secondValue;

      result.push_back(combinedRow);  // Добавляем результат в массив строк
    }
  }

  return result;  // Возвращаем результат
}
