#include <fstream>
#include <iostream>
#include <string>

#include "../headers/SYBD.hpp"
#include "../headers/Table.hpp"
#include "../headers/array.hpp"
#include "../headers/hash_map.hpp"
#include "../lib/json.hpp"

using namespace std;

int main() {
  // DB dataBase;
  // dataBase.readingConfiguration("../schema.json");
  // cout << dataBase.schemaName;

  // Инициализация хэш-таблицы
  Hash_map<string, Table, 10> structure;

  // Создаем и инициализируем объект Table
  Array<string> columns;
  columns.emplace_back("Column1");
  columns.emplace_back("Column2");

  Table table("MyTable", columns);  // Инициализация с именем и колонками

  // Печать информации о таблице
  table.print();

  // Вставка таблицы в хэш-таблицу
  try {
    structure.insert("1", table);  // Вставка с ключом "1"
  } catch (const std::runtime_error& e) {
    std::cerr << "Ошибка: " << e.what() << std::endl;
  }

  return 0;
}
