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

  // Выводим всю информацию о классе DB
  dataBase.printInfo();
  dataBase.createDirectoriesAndFiles();

  cout << "-------------------" << endl;

  SQLParser parser(dataBase);
  parser.parse("INSERT INTO Таблица1 VALUES ('somedata', '12345')");

  dataBase.printInfo();
  return 0;
}