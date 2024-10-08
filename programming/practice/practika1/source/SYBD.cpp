#include "../headers/SYBD.hpp"

#include <fstream>
#include <stdexcept>

#include "../lib/json.hpp"

using json = nlohmann::json;
using namespace std;

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

      structure.push_back(table);
    }
  } else {
    throw runtime_error("Отсутствуют необходимые ключи в JSON файле");
  }

  file.close();
};

void DB::createDirectoriesAndFiles() {
  // FIXME надо на дерикторию выше создавать папку
  //  Получаем путь к родительской директории текущего файла
  fs::path currentPath = fs::current_path();  // Получаем текущую директорию
  fs::path newFolderPath = currentPath.parent_path() /
                           "test";  // Указываем путь к новой папке "Схема 1"

  // Создаем директорию для схемы, если ее еще нет
  if (!fs::exists(schemaName)) {
    fs::create_directory(schemaName);
  }

  // Перебираем все таблицы в структуре
  for (size_t i = 0; i < structure.getSize(); ++i) {
    Table& table = structure[i];

    // Создаем директорию для таблицы
    fs::path tableDir = fs::path(schemaName) / table.tableName;
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
  }
}

void DB::printInfo() const {
  cout << "Schema Name: " << schemaName << endl;
  cout << "Tuples Limit: " << tuplesLimit << endl;
  cout << "Path to Schema: " << pathSchema << endl;
  cout << "Tables in Structure: " << structure.getSize() << endl;

  for (size_t i = 0; i < structure.getSize(); ++i) {
    cout << "Table " << (i + 1) << ":" << endl;
    structure[i].columns.print();
  }
}
