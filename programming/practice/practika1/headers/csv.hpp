#ifndef CSV_H
#define CSV_H

#include <iostream>
#include <string>

#include "../headers/array.hpp"

using namespace std;

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
};

#include "../source/csv.cpp"

#endif  // CSV_H
