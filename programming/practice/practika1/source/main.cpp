#include <fstream>
#include <iostream>
#include <string>

#include "../headers/ComandParser.hpp"
#include "../headers/SYBD.hpp"
#include "../headers/Table.hpp"
#include "../headers/array.hpp"
#include "../lib/json.hpp"

using namespace std;

int main() {
  DB dataBase;
  dataBase.readingConfiguration("../schema.json");

  dataBase.createDirectoriesAndFiles();
  cout << "-------------------" << endl;

  SQLParser parser(dataBase);
  int i = 1;
  string input;

  while (true) {
    cout << "Нажмите 1 для добавления данных, или любую другую клавишу "
            "для выхода: ";
    cin >> input;

    if (input == "1") {
      // Формируем строку с данными для вставки, где i инкрементируется
      string query = "INSERT INTO Таблица2 VALUES ('data" + to_string(i) +
                     "', 'value" + to_string(i) + "')";
      parser.parse(query);
      cout << "Добавлены данные: data" << i << " value" << i << endl;
      ++i;
      dataBase.printInfo();
      cout << endl;
    } else {
      cout << "Программа завершена." << endl;
      break;
    }
  }

  return 0;
}