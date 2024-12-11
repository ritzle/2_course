#ifndef BURSEJSONPARSER_HPP
#define BURSEJSONPARSER_HPP

#include <sstream>
#include <string>

#include "SYBD.hpp"

class BurseJsonParser {
 public:
  BurseJsonParser(DB& database);
  void parse(const std::string& query);

 private:
  DB& db;

  // Обработка POST запросов
  void handlePostUser(std::istringstream& stream);
  void handlePostOrder(std::istringstream& stream);
  void handlePostConfiguration(std::istringstream& stream);

  // Обработка GET запросов
  void handleGetOrder(std::istringstream& stream);
  void handleGetLot(std::istringstream& stream);
  void handleGetPair(std::istringstream& stream);
  void handleGetBalance(std::istringstream& stream);

  // Обработка DELETE запросов
  void handleDeleteOrder(std::istringstream& stream);
};

#include "../../source/BurseJsonParser.cpp"
#endif  // BURSEJSONPARSER_HPP
