#include "../headers/SYBD.hpp"

#include <fstream>
#include <stdexcept>

#include "../lib/json.hpp"

using json = nlohmann::json;
using namespace std;

void DB::readingConfiguration(string PathSchema) {
  std::ifstream file(PathSchema);

  if (!file.is_open()) {
    throw std::runtime_error("Не удалось открыть файл: " + PathSchema);
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

      Array<string> columns(JTable.value().size());

      for (auto& col : JTable.value()) {
        if (col.is_string()) {
          columns.emplace_back(col.get<string>());
        } else {
          throw std::runtime_error("Колонка должна быть строкой");
        }
      }

      table.columns = columns;

      // FIXME эта строчка пока все портит
      // structure.insert("1", table);
    }
  } else {
    throw std::runtime_error("Отсутствуют необходимые ключи в JSON файле");
  }

  file.close();
};
