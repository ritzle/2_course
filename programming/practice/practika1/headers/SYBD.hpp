#ifndef BD_HPP
#define BD_HPP

#include <string>

using namespace std;

class DB {
 private:
 public:
  string schemaName;
  int tuplesLimit;
  string pathSchema;

  DB(){};
  ~DB(){};

  void readingConfiguration(string PathSchema);
};
#include "../source/SYBD.cpp"
#endif