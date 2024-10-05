#include <fstream>
#include <iostream>
#include <string>

#include "../headers/SYBD.hpp"
#include "../headers/map.hpp"
#include "../lib/json.hpp"

using namespace std;

int main() {
  DB dataBase;
  dataBase.readingConfiguration("../schema.json");
  cout << dataBase.schemaName;
  return 0;
}