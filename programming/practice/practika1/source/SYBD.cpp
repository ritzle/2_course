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

  if (JFile.contains("name") && JFile.contains("tuples_limit")) {
    schemaName = JFile["name"];
    tuplesLimit = JFile["tuples_limit"];
  } else {
    throw std::runtime_error("Отсутствуют необходимые ключи в JSON файле");
  }

  file.close();
};
