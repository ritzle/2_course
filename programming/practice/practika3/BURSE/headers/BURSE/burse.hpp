
#ifndef BURSE_HPP
#define BURSE_HPP

#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "../../headers/array.hpp"
#include "../../lib/json.hpp"
#include "../SYBD/SYBD.hpp"
// #include "../SYBD/SYBD.hpp"

using namespace std;
using json = nlohmann::json;

class BurseJsonParser;

class Burse {
  friend class BurseJsonParser;

 public:
  Burse();
  ~Burse();

  string database_ip;
  int database_port;

  Array<string> lots;

  void readingConfiguration(string& PathSchema);

 private:
  // Array<string> lots;
  DB replicaDB;

  size_t _hash(const std::string& input) {
    return std::hash<std::string>{}(input);
  }
};

#include "../../source/burse.cpp"
#endif
