#include "../headers/SYBD/BurseJsonParser.hpp"

#include <boost/beast.hpp>
#include <boost/json.hpp>

BurseJsonParser::BurseJsonParser(DB& database) : db(database) {}

void BurseJsonParser::parse(const std::string& jsonStr) {
  try {
    // Парсим строку JSON в объект
    nlohmann::json jsonObj = nlohmann::json::parse(jsonStr);

    // Выводим распарсенный JSON для отладки
    std::cout << "Parsed JSON: " << jsonObj.dump(4) << std::endl;

    // В зависимости от данных в JSON, вызывать различные обработчики
    std::string method = jsonObj["method"];
    std::string path = jsonObj["path"];

    std::cout << "Method: " << method << ", Path: " << path << std::endl;

    // В зависимости от значения "method" обрабатываем POST, GET или DELETE
    if (method == "POST") {
      if (path == "/user") {
        handlePostUser(jsonObj["data"]);
      } else if (path == "/order") {
        handlePostOrder(jsonObj["data"]);
      } else if (path == "/config") {
        handlePostConfiguration(jsonObj);
      } else {
        throw std::invalid_argument("Unknown POST path: " + path);
      }
    } else if (method == "GET") {
      if (path == "/order") {
        handleGetOrder(jsonObj["data"]);
      } else if (path == "/lot") {
        handleGetLot(jsonObj["data"]);
      } else if (path == "/pair") {
        handleGetPair(jsonObj["data"]);
      } else if (path == "/balance") {
        handleGetBalance(jsonObj["data"]);
      } else {
        throw std::invalid_argument("Unknown GET path: " + path);
      }
    } else if (method == "DELETE") {
      if (path == "/order") {
        handleDeleteOrder(jsonObj["data"]);
      } else {
        throw std::invalid_argument("Unknown DELETE path: " + path);
      }
    } else {
      throw std::invalid_argument("Unknown HTTP method: " + method);
    }

  } catch (const std::exception& e) {
    std::cerr << "Error processing JSON: " << e.what() << std::endl;
  }
}

// Пример реализации метода для обработки POST запроса на создание пользователя
void BurseJsonParser::handlePostUser(const nlohmann::json& jsonData) {
  cout << "Post user" << endl;
}

// Пример метода для создания ответа от базы данных
std::string BurseJsonParser::createDatabaseResponse(
    const std::string& queryResult) {
  cout << "----" << endl;
  return "---";
}

// Пример метода для обработки POST запроса на создание ордера
void BurseJsonParser::handlePostOrder(const nlohmann::json& jsonData) {
  cout << "Post order" << endl;
}

// Пример метода для обработки POST запроса на настройку
void BurseJsonParser::handlePostConfiguration(const nlohmann::json& jsonData) {
  std::string config_value = jsonData["config_value"];
  db.updateConfigurationBurse(config_value);

  // cout << "--" << config_value << "---\n";
}

// Пример метода для обработки GET запроса на ордер
void BurseJsonParser::handleGetOrder(const nlohmann::json& jsonData) {
  cout << "Get order" << endl;
}

// Пример метода для обработки GET запроса на лот
void BurseJsonParser::handleGetLot(const nlohmann::json& jsonData) {
  cout << "Get Lot" << endl;
}

// Пример метода для обработки GET запроса на пару
void BurseJsonParser::handleGetPair(const nlohmann::json& jsonData) {
  cout << "GetPair" << endl;
}

// Пример метода для обработки GET запроса на баланс
void BurseJsonParser::handleGetBalance(const nlohmann::json& jsonData) {
  cout << "GetBalance" << endl;
}

// Пример метода для обработки DELETE запроса на удаление ордера
void BurseJsonParser::handleDeleteOrder(const nlohmann::json& jsonData) {
  cout << "delete Order" << endl;
}
