#ifndef BD_HPP
#define BD_HPP

#include <string>

#include "../headers/array.hpp"
#include "../headers/hash_map.hpp"
#include "Table.hpp"

using namespace std;

class DB {
 private:
 public:
  string schemaName;
  int tuplesLimit;
  string pathSchema;
  Hash_map<string, Table, 10> structure;

  DB(){};
  ~DB(){};

  void readingConfiguration(string PathSchema);
};
#include "../source/SYBD.cpp"
#endif