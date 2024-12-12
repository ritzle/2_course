#ifndef BURSEJSONPARSER_HPP
#define BURSEJSONPARSER_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "SYBD.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace beast = boost::beast;
namespace http = beast::http;

class BurseJsonParser {
 public:
  // Конструктор, который принимает объект DB
  BurseJsonParser(DB& database);

  // Метод для обработки запроса
  void parse(const std::string& jsonStr);

 private:
  DB& db;

  // Обработка POST запросов
  void handlePostUser(const nlohmann::json& jsonData);
  void handlePostOrder(const nlohmann::json& jsonData);
  void handlePostConfiguration(const nlohmann::json& jsonData);

  // Обработка GET запросов
  void handleGetOrder(const nlohmann::json& jsonData);
  void handleGetLot(const nlohmann::json& jsonData);
  void handleGetPair(const nlohmann::json& jsonData);
  void handleGetBalance(const nlohmann::json& jsonData);

  // Обработка DELETE запросов
  void handleDeleteOrder(const nlohmann::json& jsonData);

  // Метод для формирования ответа от базы данных
  std::string createDatabaseResponse(const std::string& queryResult);
};

#include "../../source/BurseJsonParser.cpp"

#endif  // BURSEJSONPARSER_HPP
