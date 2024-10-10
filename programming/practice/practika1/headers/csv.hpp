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

  CSV(const string& name, const Array<string>& cols);

  int countLine();
};

#include "../source/csv.cpp"

#endif  // CSV_H
