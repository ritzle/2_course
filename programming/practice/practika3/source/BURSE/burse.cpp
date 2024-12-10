#include "../../headers/BURSE/burse.hpp"

Burse::Burse() {
  db.readingConfiguration(
      "/home/vlad/vsCode/2_course/programming/practice/practika3/schema.json");
}

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

// FIXME
void Burse::loadingConfiguration() {
  // lot
  Array<string> values;
  for (int i = 0; i < lots.getSize(); i++) {
    string nameLot = lots[i];

    values.push_back(to_string(i + 1));
    values.push_back(nameLot);

    db.insertIntoTable("lot", values);
    values.clear();
  }

  // pair
  int coutPair = 1;
  for (int i = 1; i <= lots.getSize(); ++i) {
    for (int j = 1; j <= lots.getSize(); ++j) {
      values.push_back(to_string(coutPair++));
      values.push_back(to_string(i));
      values.push_back(to_string(j));

      db.insertIntoTable("pair", values);
      values.clear();
    }
  }
}