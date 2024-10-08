#ifndef BD_HPP
#define BD_HPP

#include <filesystem>
#include <string>

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
  int pk_sequence;

  DB();
  ~DB(){};

  void readingConfiguration(string PathSchema);
  void createDirectoriesAndFiles();
  void printInfo() const;
  void insertIntoTable(string TableName, Array<string> values);

 private:
  Table& searchTable(const string& TableName);
};
#include "../source/SYBD.cpp"
#endif