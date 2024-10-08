#include "../headers/SYBD.hpp"

#include <fstream>
#include <stdexcept>

#include "../lib/json.hpp"

using json = nlohmann::json;
using namespace std;

DB::DB() : pk_sequence(0) {}

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

  file.close();
};

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

    fs::path pkSeqenceFile = tableDir / (table.tableName + "pk_seqence");
    if (!fs::exists(pkSeqenceFile)) {
      ofstream file(pkSeqenceFile);
      if (file.is_open()) {
        file << pk_sequence;
        file.close();
      } else {
        std::cerr << "Ошибка при создании файла: "
                  << table.tableName + "pk_seqence" << std::endl;
      }
    }
  }
}

// TODO добавить обновление  CSV файлов + pk_seqence

// TODO незобыть добавить проверку(и создание) на случай tuplesLimit;
void DB::insertIntoTable(string TableName, Array<string> arrValues) {
  Table& currentTable = searchTable(TableName);

  // TODO может конечно надо сдеать получше но пока как естьж
  ++pk_sequence;
  arrValues.insert_beginning(to_string(pk_sequence));

  currentTable.line.push_back(arrValues);
}

Table& DB::searchTable(const string& TableName) {
  for (auto& table : structure) {
    if (table.tableName == TableName) {
      return table;
    }
  }

  throw std::runtime_error("Table not found: " + TableName);
}

// FIXME добваить вывод линий
void DB::printInfo() const {
  cout << "Schema Name: " << schemaName << endl;
  cout << "Tuples Limit: " << tuplesLimit << endl;
  cout << "Path to Schema: " << pathSchema << endl;
  cout << "Tables in Structure: " << structure.getSize() << endl;
  cout << "pk_sequence: " << pk_sequence << endl;

  for (size_t i = 0; i < structure.getSize(); ++i) {
    cout << "Table " << (i + 1) << ": " << structure[i].tableName << endl;

    // Вывод колонок таблицы
    cout << "Columns: ";
    structure[i].columns.print();

    // Вывод строк данных
    for (auto& line : structure[i].line) {
      line.print();
    }
  }
  cout << endl;
}
