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

 private:
  Table& searchTable(const string& TableName);
  void printFromQuery(const string& query);
  string trim(const string& str);

  void loadExistingSchemaData();
  Array<string> parseCSVLine(const string& line);

  // TODO можно конечн заченить на просто путь и значение но ....
  // FIXME не совсем коректное название возможно, подрозумевается запись в фаил
  void updatePkSeqence(Table& table);
  void updateCSVFile(Table& tableб);
  void updateLock(Table& table);
};
#include "../source/SYBD.cpp"
#endif