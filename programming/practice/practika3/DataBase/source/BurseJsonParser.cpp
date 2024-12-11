#include "../headers/SYBD/BurseJsonParser.hpp"

#include <iostream>
#include <stdexcept>

BurseJsonParser::BurseJsonParser(DB& database) : db(database) {}

void BurseJsonParser::parse(const std::string& query) {
  std::istringstream stream(query);
  std::string method, path;
  stream >> method >> path;  // Считываем HTTP-метод и путь запроса

  if (method == "POST") {
    if (path == "/user") {
      handlePostUser(stream);
    } else if (path == "/order") {
      handlePostOrder(stream);
    } else if (path == "/configuration") {
      handlePostConfiguration(stream);
    } else {
      std::cerr << "Неизвестный путь для POST: " + path << std::endl;
    }
  } else if (method == "GET") {
    if (path == "/order") {
      handleGetOrder(stream);
    } else if (path == "/lot") {
      handleGetLot(stream);
    } else if (path == "/pair") {
      handleGetPair(stream);
    } else if (path == "/balance") {
      handleGetBalance(stream);
    } else {
      std::cerr << "Неизвестный путь для GET: " + path << std::endl;
    }
  } else if (method == "DELETE") {
    if (path == "/order") {
      handleDeleteOrder(stream);
    } else {
      std::cerr << "Неизвестный путь для DELETE: " + path << std::endl;
    }
  } else {
    std::cerr << "Неизвестный HTTP-метод: " + method << std::endl;
  }
}

// Обработка POST запросов
void BurseJsonParser::handlePostUser(std::istringstream& stream) {
  std::string userData;
  std::getline(stream, userData);
  // Парсим и вставляем данные о пользователе
  // db.insertUser(userData);
}

void BurseJsonParser::handlePostOrder(std::istringstream& stream) {
  std::string orderData;
  std::getline(stream, orderData);
  // Парсим и добавляем новый заказ
  // db.insertOrder(orderData);
}

void BurseJsonParser::handlePostConfiguration(std::istringstream& stream) {
  std::string configData;
  std::getline(stream, configData);
  // Обрабатываем и обновляем конфигурацию
  db.updateConfigurationBurse(configData);
}

// Обработка GET запросов
void BurseJsonParser::handleGetOrder(std::istringstream& stream) {
  // Получаем данные о заказах
  // std::string orderData = db.getOrders();
  // std::cout << "Ответ на запрос GET /order: " << orderData << std::endl;
}

void BurseJsonParser::handleGetLot(std::istringstream& stream) {
  // Получаем данные о лотах
  // std::string lotData = db.getLots();
  // std::cout << "Ответ на запрос GET /lot: " << lotData << std::endl;
}

void BurseJsonParser::handleGetPair(std::istringstream& stream) {
  // Получаем данные о парах
  // std::string pairData = db.getPairs();
  // std::cout << "Ответ на запрос GET /pair: " << pairData << std::endl;
}

void BurseJsonParser::handleGetBalance(std::istringstream& stream) {
  // Получаем информацию о балансе
  // std::string balanceData = db.getBalance();
  // std::cout << "Ответ на запрос GET /balance: " << balanceData << std::endl;
}

// Обработка DELETE запросов
void BurseJsonParser::handleDeleteOrder(std::istringstream& stream) {
  std::string orderId;
  stream >> orderId;
  // Удаляем заказ по идентификатору
  // db.deleteOrder(orderId);
}
