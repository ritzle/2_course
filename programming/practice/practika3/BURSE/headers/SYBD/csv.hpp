#ifndef CSV_H
#define CSV_H

#include <iostream>
#include <sstream>
#include <string>

#include "../../lib/json.hpp"
#include "../array.hpp"
using namespace std;
using json = nlohmann::json;

class CSV {
 public:
  string csvName;
  Array<string> columns;
  Array<Array<string>> line;

  CSV();

  CSV(const string name, const Array<string> cols);
  CSV(const string& name);

  int countLine();

  string getColumnValue(const string& columnName, int rowIndex) const;
  json serialize() const;
  CSV deserialize(const json& j);
};

#include "../../source/csv.cpp"

#endif  // CSV_H
