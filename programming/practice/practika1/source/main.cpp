// #include <fstream>
// #include <iostream>
// #include <string>

// #include "../headers/ComandParser.hpp"
// #include "../headers/SYBD.hpp"
// #include "../headers/Table.hpp"
// #include "../headers/array.hpp"
// #include "../lib/json.hpp"

// using namespace std;

// int main() {
//   DB dataBase;
//   dataBase.readingConfiguration("../schema.json");
//   cout << "-------------------" << endl;

//   SQLParser parser(dataBase);
//   int i = 1;
//   string input;

//   while (true) {
//     cout << "Нажмите 1 для добавления данных, 2 для выполнения запроса WHERE,
//     "
//             "или любую другую клавишу для выхода: ";
//     cin >> input;

//     if (input == "1") {
//       // Формируем строку с данными для вставки, где i инкрементируется
//       string query = "INSERT INTO Таблица2 VALUES ('data" + to_string(i) +
//                      "', 'value" + to_string(i) + "')";
//       parser.parse(query);
//       cout << "Добавлены данные: data" << i << " value" << i << endl;
//       ++i;
//       dataBase.printInfo();
//       cout << endl;
//     } else if (input == "2") {
//       // Запрос с условием WHERE
//       string query =
//           "SELECT таблица1.колонка1, таблица2.колонка1 FROM Таблица1 Таблица2
//           " "WHERE Таблица2.колонка1 = Таблица1.колонка2 AND
//           Таблица2.колонка1 = " "5 OR Таблица1.колонка2 = " "5";

//       parser.parse(query);
//       cout << endl;
//     } else if (input == "3") {
//       // Запрос с условием WHERE
//       string query =
//           "DELETE FROM Таблица1, Таблица2 WHERE Таблица2.колонка1 = "
//           "Таблица1.колонка1 OR Таблица1.колонка2 = 5";

//       parser.parse(query);
//       cout << endl;

//     } else if (input == "4") {
//       dataBase.printInfo();
//     } else {
//       cout << "Программа завершена." << endl;
//       break;
//     }
//   }

//   return 0;
// }
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
  cout << "-------------------" << endl;

  SQLParser parser(dataBase);
  string input;

  while (true) {
    cout << "Введите SQL-запрос или 'exit' для выхода: ";
    getline(cin, input);  // позволяет вводить длинные строки

    if (input == "exit") {
      cout << "Программа завершена." << endl;
      break;
    }

    parser.parse(input);
    cout << endl;

    // Для быстрого копирования запросов:
    // Пример INSERT:
    // INSERT INTO Таблица2 VALUES ('data1', 'value1')

    // Пример SELECT с WHERE:
    // SELECT Таблица1.колонка1, Таблица2.колонка1 FROM Таблица1, Таблица2
    // SELECT Таблица1.колонка1,Таблица1.колонка1,Таблица2.колонка1 FROM
    // Таблица1, Таблица2 WHERE Таблица2.колонка1 = Таблица1.колонка2 AND
    // Таблица2.колонка1 = 5 OR Таблица1.колонка2 = 5

    // Пример DELETE:
    // DELETE FROM Таблица1, Таблица2 WHERE Таблица2.колонка1 =
    // Таблица1.колонка1 OR Таблица1.колонка2 = 5

    // DELETE FROM Таблица2 WHERE Таблица2.колонка2 = value1
  }

  return 0;
}
