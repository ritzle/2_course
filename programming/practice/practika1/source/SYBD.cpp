#include "../headers/SYBD.hpp"

#include <fstream>
#include <stdexcept>

#include "../lib/json.hpp"

using json = nlohmann::json;
using namespace std;

DB::DB(){};

#include <filesystem>  // Для проверки существования директории

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

      for (auto& col : JTable.value()) {
        if (col.is_string()) {
          table.columns.push_back(col.get<string>());
        } else {
          throw runtime_error("Колонка должна быть строкой");
        }
      }
      table.columns.insert_beginning(table.tableName + "_pk");

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
};

// Метод для чтения данных из существующей схемы
void DB::loadExistingSchemaData() {
  for (auto& table : structure) {
    table.pathTable = fs::path("..") / schemaName / table.tableName;

    // 1. Считаем количество CSV файлов для таблицы
    table.countCSVFiles();

    // 2. Читаем информацию из файла блокировки и pk_sequence
    table.readLockFile();
    table.readPKSequenceFile();

    // 3. Загружаем данные из всех CSV файлов
    for (int i = 1; i <= table.countCSVFile; ++i) {
      string csvFilePath = table.pathTable + "/" + to_string(i) + ".csv";
      ifstream csvFile(csvFilePath);

      if (!csvFile.is_open()) {
        throw runtime_error("Не удалось открыть файл таблицы: " + csvFilePath);
      }

      string line;
      bool isFirstLine = true;  // Флаг для отслеживания первой строки
      while (getline(csvFile, line)) {
        if (isFirstLine) {
          isFirstLine = false;  // Пропускаем первую строку
          continue;  // Переходим к следующей итерации
        }

        Array<string> row = parseCSVLine(line);
        table.line.push_back(row);  // Сохраняем строку в rows
      }

      csvFile.close();  // Закрываем файл после обработки
    }

    // Проверяем и выводим статус блокировки таблицы
    if (table.lock) {
      cout << "Таблица " << table.tableName << " заблокирована." << endl;
    }

    cout << "Таблица " << table.tableName
         << " загружена успешно. Количество строк: " << table.line.getSize()
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
      for (size_t j = 0; j < table.columns.getSize(); ++j) {
        file << table.columns[j];
        if (j < table.columns.getSize() - 1) {
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
        cerr << "Ошибка при создании файла: " << table.tableName + "pk_seqence"
             << endl;
      }
    }
  }
}

// TODO незобыть добавить проверку(и создание) на случай tuplesLimit;
void DB::insertIntoTable(string TableName, Array<string> arrValues) {
  Table& currentTable = searchTable(TableName);

  currentTable.readLockFile();

  // Ожидание разблокировки таблицы
  while (currentTable.lock == 1) {
    this_thread::sleep_for(chrono::milliseconds(100));
    currentTable.readLockFile();
    cout << "жду разблокировки" << endl;
  }

  // Блокируем таблицу для работы
  currentTable.lock = 1;
  updateLock(currentTable);

  ++currentTable.pk_sequence;
  arrValues.insert_beginning(to_string(currentTable.pk_sequence));

  currentTable.line.push_back(
      arrValues);  // добовление значение непосредственно в структуру
  updatePkSeqence(currentTable);
  updateCSVFile(currentTable);

  // Разблокируем таблицу для других операций
  currentTable.lock = 0;
  updateLock(currentTable);
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
  ofstream out(table.pathTable + "/" + to_string(table.countCSVFile) + ".csv");

  if (out.is_open()) {
    for (size_t i = 0; i < table.columns.getSize(); ++i) {
      out << table.columns[i];
      if (i < table.columns.getSize() - 1) {
        out << ",";
      }
    }
    out << endl;

    if (table.line.getSize() == 0) {
      cerr << "Нет данных для записи в строки!" << endl;
    }

    for (size_t j = 0; j < table.line.getSize(); ++j) {
      for (size_t k = 0; k < table.line[j].getSize(); ++k) {
        out << table.line[j][k];

        if (k < table.line[j].getSize() - 1) {
          out << ",";
        }
      }
      out << endl;
    }

    out.close();
  } else {
    cerr << "Ошибка при работе с файлом: " << table.pathTable << "/"
         << to_string(table.countCSVFile) << ".csv" << endl;
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
    structure[i].columns.print();

    // Вывод строк данных
    for (auto& line : structure[i].line) {
      line.print();
    }

    cout << endl;
  }
  cout << endl;
}
