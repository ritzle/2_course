#include "../headers/SYBD/BurseJsonParser.hpp"

#include <boost/beast.hpp>
#include <boost/json.hpp>

BurseJsonParser::BurseJsonParser(DB& database, SQLParser& parser)
    : db(database), SQLparser(parser) {}

json BurseJsonParser::parse(const std::string& jsonStr) {
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
        string key_user = handlePostUser(jsonObj);

        json response;
        response["request"] = "POST /user";

        if (key_user != "error") {
          response["request_status"] = "completed";
          response["user_key"] = key_user;
        } else {
          response["request_status"] = "error";
          response["user_key"] = key_user;
        }

        return response;
      } else if (path == "/order") {
        handlePostOrder(jsonObj);
      } else if (path == "/config") {
        handlePostConfiguration(jsonObj);
      } else {
        throw std::invalid_argument("Unknown POST path: " + path);
      }
    }
    // TODO хз выглядит как костыть так как быиржа до этого получает инфу с бд и
    // формирует ответ у себя (но из-за ного как работает сервер он доджен
    // что-то бд отправит и тольок с бдприходит ответ на биржу и биржа клиенту)
    else if (method == "GET") {  //
      if (path == "/order") {
        handleGetOrder(jsonObj);
      } else if (path == "/lot") {
        return jsonObj;
      } else if (path == "/pair") {
        return jsonObj;
      } else if (path == "/balance") {
        return jsonObj;
      } else if (path == "/db") {
        string tables = jsonObj["tables"].get<string>();
        json otvet = handleGetDataBase(tables);

        istringstream ss(tables);

        string TableName;

        while (ss >> TableName) {
          db.unloadSchemaData(TableName);
        }

        return otvet;
      } else {
        throw std::invalid_argument("Unknown GET path: " + path);
      }
    } else if (method == "DELETE") {
      if (path == "/order") {
        handleDeleteOrder(jsonObj);
      } else {
        throw std::invalid_argument("Unknown DELETE path: " + path);
      }
    } else {
      throw std::invalid_argument("Unknown HTTP method: " + method);
    }

  } catch (const std::exception& e) {
    std::cerr << "Error processing JSON: " << e.what() << std::endl;
  }

  json j;
  return j;
}

// Пример реализации метода для обработки POST запроса на создание пользователя
string BurseJsonParser::handlePostUser(const nlohmann::json& jsonData) {
  try {
    // Проверяем наличие поля "commands" и его тип
    if (!jsonData.contains("command")) {
      throw std::runtime_error("Missing 'commands' in JSON");
    }
    if (!jsonData["command"].is_string()) {
      throw std::runtime_error("'commands' must be a string");
    }

    string user_key = jsonData["user_key"].get<std::string>();

    std::string commands = jsonData["command"].get<std::string>();
    std::istringstream stream(commands);
    std::string currentCommand;

    while (std::getline(stream, currentCommand, '\n')) {
      if (!currentCommand.empty()) {
        SQLparser.parse(currentCommand);  // Парсим каждую команду
      }
    }

    return user_key;
  } catch (const std::exception& e) {
    std::cerr << "Error processing POST configuration: " << e.what()
              << std::endl;
  }

  return "error";
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

// конфигурация
void BurseJsonParser::handlePostConfiguration(const nlohmann::json& jsonData) {
  try {
    // Проверяем наличие поля "commands" и его тип
    if (!jsonData.contains("command")) {
      throw std::runtime_error("Missing 'commands' in JSON");
    }
    if (!jsonData["command"].is_string()) {
      throw std::runtime_error("'commands' must be a string");
    }

    string TableName = "lot";
    db.loadExistingSchemaData(TableName);
    Table& currentTable = db.searchTable(TableName);

    if (currentTable.csv.back().line.getSize() == 0) {
      // Извлекаем строку из JSON
      std::string commands = jsonData["command"].get<std::string>();
      std::istringstream stream(commands);
      std::string currentCommand;

      // Разбиваем строку на команды и обрабатываем
      while (std::getline(stream, currentCommand, '\n')) {
        if (!currentCommand.empty()) {
          SQLparser.parse(currentCommand);  // Парсим каждую команду
        }
      }
    } else {
      cout
          << "таблица lot и pair не пустая, конфигурация не будет загруженна\n";
    }

    db.unloadSchemaData(TableName);

  } catch (const std::exception& e) {
    std::cerr << "Error processing POST configuration: " << e.what()
              << std::endl;
  }
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

json BurseJsonParser::handleGetDataBase(string tables) {
  istringstream ss(tables);

  string TableName;

  while (ss >> TableName) {
    db.loadExistingSchemaData(TableName);
  }

  return db.serialize();
}