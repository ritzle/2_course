#ifndef SQLPARSER_HPP
#define SQLPARSER_HPP

#include <sstream>
#include <stdexcept>
#include <string>

#include "SYBD.hpp"
#include "array.hpp"
using namespace std;

class SQLParser {
 public:
  SQLParser(DB& database);

  void parse(const string& query);

 private:
  DB& db;

  void handleInsert(istringstream& stream);

  void handleSelect(istringstream& stream);

  void handleDelete(istringstream& stream);

  void handleWhere(istringstream& stream);

  // для списка значений в insert
  Array<string> parseValues(const string& valuesList);
};

#include "../source/ComandParser.cpp"

#endif
