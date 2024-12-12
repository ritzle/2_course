#include "../headers/BURSE/burse.hpp"

Burse::Burse() {}

Burse::~Burse() {}

void Burse::readingConfiguration(string& pathSchema) {
  ifstream file(pathSchema);

  if (!file.is_open()) {
    throw runtime_error("Не удалось открыть файл: " + pathSchema);
  }

  json jFile;
  file >> jFile;

  if (jFile.contains("lots") && jFile.contains("database_ip") &&
      jFile.contains("database_port")) {
    // Загружаем активы

    for (auto& lot : jFile["lots"]) {
      if (lot.is_string()) {
        lots.push_back(lot.get<string>());
      } else {
        throw runtime_error("Колонка должна быть строкой");
      }
    }

    // Загружаем данные о базе данных
    database_ip = jFile["database_ip"];
    database_port = jFile["database_port"];

  } else {
    throw runtime_error("Отсутствуют необходимые ключи в JSON файле");
  }

  file.close();
}
