#ifndef BD_HPP
#define BD_HPP

#include <chrono>
#include <filesystem>
#include <string>
#include <thread>

#include "Table.hpp"
#include "array.hpp"

using namespace std;
namespace fs = filesystem;

class DB {
 private:
 public:
  string schemaName;
  int tuplesLimit;
  string pathSchema;
  Array<Table> structure;

  DB();
  ~DB(){};

  void readingConfiguration(string PathSchema);
  void createDirectoriesAndFiles();
  void printInfo() const;

  void insertIntoTable(string TableName, Array<string> values);
  void applyWhereConditions(const Array<Array<string>>& conditional);
  void applyDeleteConditions(string& tableName,
                             Array<Array<string>>& conditions);

 private:
  Table& searchTable(const string& TableName);
  string trim(const string& str);

  void loadExistingSchemaData();
  Array<string> parseCSVLine(const string& line);

  // TODO надо ли все эти функции в класс Table перенести???
  // FIXME не совсем коректное название возможно, подрозумевается запись в фаил
  void updatePkSeqence(Table& table);
  void updateCSVFile(Table& table);  // только для вставки
  void updateLock(Table& table);
  void rewriteAllCSVFiles(Table& table);
  void rewriteFil(Table& table, int numberCsv);  // для rewriteCSVFile
};
#include "../source/SYBD.cpp"
#endif