#include "../headers/BURSE/jsonManagerBurse.hpp"

#include <iostream>
#include <sstream>

BurseJsonParser::BurseJsonParser() {}
BurseJsonParser::~BurseJsonParser() {}

json BurseJsonParser::handle_request(const std::string& request) {
  std::istringstream stream(request);
  std::string method, path;
  stream >> method >> path;  // Получаем HTTP метод и путь

  if (method == "POST") {
    if (path == "/user") {
      std::string username, key;
      stream >> username >> key;
      return create_user(username, key);
    } else if (path == "/order") {
      std::string user_key;
      int pair_id;
      float quantity, price;
      std::string type;
      stream >> user_key >> pair_id >> quantity >> price >> type;
      return create_order(user_key, pair_id, quantity, price, type);
    } else if (path == "/config") {
      std::string config_name, config_value;
      config_name = "тест_имя";
      config_value = "тест_значение";
      // stream >> config_name >> config_value;
      return set_configuration(config_name, config_value);
    } else {
      throw std::invalid_argument("Неизвестный путь для POST: " + path);
    }
  } else if (method == "GET") {
    if (path == "/order") {
      return get_order();
    } else if (path == "/lot") {
      return get_lot();
    } else if (path == "/pair") {
      return get_pair();
    } else if (path == "/balance") {
      std::string user_key;
      stream >> user_key;
      return get_balance(user_key);
    } else {
      throw std::invalid_argument("Неизвестный путь для GET: " + path);
    }
  } else if (method == "DELETE") {
    if (path == "/order") {
      std::string user_key;
      int order_id;
      stream >> user_key >> order_id;
      return delete_order(user_key, order_id);
    } else {
      throw std::invalid_argument("Неизвестный путь для DELETE: " + path);
    }
  } else {
    throw std::invalid_argument("Неизвестный HTTP метод: " + method);
  }
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

json BurseJsonParser::set_configuration(const std::string& config_name,
                                        const std::string& config_value) {
  json j;
  j["config_name"] = config_name;
  j["config_value"] = config_value;
  return j;
}
