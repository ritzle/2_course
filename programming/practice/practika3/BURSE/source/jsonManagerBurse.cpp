#include "../headers/BURSE/jsonManagerBurse.hpp"

#include <iostream>
#include <sstream>

BurseJsonParser::BurseJsonParser(Burse& burse) : burse(burse) {}
BurseJsonParser::~BurseJsonParser() {}

#include <sstream>
#include <stdexcept>

json BurseJsonParser::handle_request(
    const http::request<http::string_body>& req) {
  std::string method = std::string(req.method_string());
  std::string path = std::string(req.target());

  // Создаем поток для чтения из тела запроса
  std::istringstream stream(req.body());

  if (method == "POST") {
    if (path == "/user") {
      std::string username, key;
      stream >> username >> key;
      return create_user(username, key);
    }

    if (path == "/order") {
      std::string user_key, type;
      int pair_id;
      float quantity, price;
      stream >> user_key >> pair_id >> quantity >> price >> type;
      return create_order(user_key, pair_id, quantity, price, type);
    }

    if (path == "/config") {
      std::string config_name = "lot user";
      // stream >> config_name >> config_value;
      return set_configuration(config_name);
    }

    throw std::invalid_argument("Неизвестный путь для POST: " + path);
  }

  if (method == "GET") {
    if (path == "/order") {
      return get_order();
    }

    if (path == "/lot") {
      return get_lot();
    }

    if (path == "/pair") {
      return get_pair();
    }

    if (path == "/balance") {
      std::string user_key;
      stream >> user_key;
      return get_balance(user_key);
    }

    throw std::invalid_argument("Неизвестный путь для GET: " + path);
  }

  if (method == "DELETE") {
    if (path == "/order") {
      std::string user_key;
      int order_id;
      stream >> user_key >> order_id;
      return delete_order(user_key, order_id);
    }

    throw std::invalid_argument("Неизвестный путь для DELETE: " + path);
  }

  throw std::invalid_argument("Неизвестный HTTP метод: " + method);
}

json BurseJsonParser::create_user(const std::string& username,
                                  const std::string& key) {
  json j;
  j["username"] = username;
  j["key"] = key;
  return j;
}

json BurseJsonParser::create_order(const std::string& user_key, int pair_id,
                                   float quantity, float price,
                                   const std::string& type) {
  json j;
  j["user_key"] = user_key;
  j["pair_id"] = pair_id;
  j["quantity"] = quantity;
  j["price"] = price;
  j["type"] = type;
  return j;
}

json BurseJsonParser::get_order() {
  json j;
  // Пример данных для возврата (можно адаптировать)
  j["order_id"] = 1234;
  return j;
}

json BurseJsonParser::delete_order(const std::string& user_key, int order_id) {
  json j;
  j["user_key"] = user_key;
  j["order_id"] = order_id;
  j["status"] = "deleted";
  return j;
}

json BurseJsonParser::get_lot() {
  json j;
  // Пример данных для лотов
  j["lot_id"] = 5678;
  return j;
}

json BurseJsonParser::get_pair() {
  json j;
  j["pair_id"] = 1;
  j["pair_name"] = "BTC/USD";
  return j;
}

json BurseJsonParser::get_balance(const std::string& user_key) {
  json j;
  j["user_key"] = user_key;
  j["balance"] = 1000.50;
  return j;
}

json BurseJsonParser::set_configuration(const std::string& config_name) {
  json j;
  j["method"] = "POST";
  j["path"] = "/config";
  j["config_name"] = config_name;
  j["config_value"] = burse.lots.to_stringJson();
  return j;
}
