#include "../headers/SYBD.hpp"

#include <filesystem>  // Для проверки существования директории
#include <fstream>
#include <stdexcept>

#include "../lib/json.hpp"

using json = nlohmann::json;
using namespace std;

DB::DB(){};

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

      CSV csv;  // Создаем объект CSV для хранения данных
      for (auto& col : JTable.value()) {
        if (col.is_string()) {
          csv.columns.push_back(col.get<string>());
        } else {
          throw runtime_error("Колонка должна быть строкой");
        }
      }
      table.csv.push_back(csv);  // Добавляем CSV объект в таблицу

      table.csv[0].columns.insert_beginning(table.tableName + "_pk");

      structure.push_back(table);
    }
  } else {
    throw runtime_error("Отсутствуют необходимые ключи в JSON файле");
  }

  // Проверяем, существует ли директория схемы
  string schemaPath = "../" + schemaName;
  if (filesystem::exists(schemaPath)) {
    // Если директория существует, загружаем данные существующей схемы
    cout << "Схема " << schemaName << " уже существует. Загружаем данные..."
         << endl;
    loadExistingSchemaData();
  } else {
    createDirectoriesAndFiles();
  }

  file.close();
}

void DB::loadExistingSchemaData() {
  for (auto& table : structure) {
    table.pathTable = fs::path("..") / schemaName / table.tableName;

    // 1. Считаем количество CSV файлов для таблицы
    table.countCSVFiles();

    // 2. Читаем информацию из файла блокировки и pk_sequence
    table.readLockFile();
    table.readPKSequenceFile();

    // 3. Инициализируем нужное количество CSV объектов
    while (table.csv.getSize() < table.countCSVFile) {
      table.csv.push_back(CSV());
    }

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
  arrValues.insert_beginning(to_string(currentTable.pk_sequence));

  // Проверяем, достиг ли лимит строк в текущем CSV файле
  if (currentTable.csv.back().line.getSize() >= tuplesLimit) {
    // Создаем новый CSV файл
    string newCsvName = to_string(currentTable.csv.getSize() + 1) + ".csv";
    CSV newCsv(newCsvName, currentTable.csv.back().columns);
    currentTable.csv.push_back(newCsv);
    currentTable.countCSVFile++;  // Обновляем количество CSV файлов
  }

  // Добавляем строку в последний CSV
  currentTable.csv.back().line.push_back(arrValues);

  // Обновляем только один раз после завершения вставки
  updateCSVFile(currentTable);

  // Разблокируем таблицу
  currentTable.lock = 0;
  updateLock(currentTable);
  updatePkSeqence(currentTable);
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