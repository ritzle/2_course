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

  cout << endl << req << endl;

  // Проверяем наличие нужных заголовков
  std::string user_secret_key;
  if (req.find("X-USER-KEY") != req.end()) {
    user_secret_key = std::string(req["X-USER-KEY"]);
  }

  std::string username;
  if (req.find("X-USER-NAME") != req.end()) {
    username = std::string(req["X-USER-NAME"]);
  }

  // Создаем поток для чтения из тела запроса
  std::istringstream stream(req.body());

  if (method == "POST") {
    if (path == "/user") {
      // тыблицы которые отправил БД
      string name_table = "user lot";

      send_request_to_db(db_ip, db_port, name_table);

      Table& currentTable = burse.replicaDB.searchTable("user");
      int index_columnId =
          burse.replicaDB.findColumnIndex(currentTable.csv[0], "user_id");

      // проверка на 1 пользователя
      string user_id;
      if (currentTable.csv.back().line.empty()) {
        user_id = "1";
      } else {
        user_id = to_string(
            stoi(currentTable.csv.back().line.back()[index_columnId]) +
            1);  // на 1 больше чем последнее в бд
      }

      string user_key = std::to_string(burse._hash(username + user_id));

      return create_user(username, user_key, user_id);
    }

    if (path == "/order") {
      // FIXME не сделан
      std::string user_key, type;
      int pair_id;
      float quantity, price;
      stream >> user_key >> pair_id >> quantity >> price >> type;
      return create_order(user_key, pair_id, quantity, price, type);
    }

    if (path == "/config") {
      std::string config_name = "lot pair";
      // stream >> config_name >> config_value;
      return set_configuration(config_name);
    }

    throw std::invalid_argument("Неизвестный путь для POST: " + path);
  }

  if (method == "GET") {
    if (path == "/order") {
      // FIXME не сделан
      return get_order();
    }

    if (path == "/lot") {
      return get_lot();
    }

    if (path == "/pair") {
      return get_pair();
    }

    if (path == "/balance") {
      return get_balance(user_secret_key);
    }

    throw std::invalid_argument("Неизвестный путь для GET: " + path);
  }

  if (method == "DELETE") {
    // FIXME не сделан
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

// FIME не знаю буду ли делать но убрать логику заполнения баланася в другу
// функцию
json BurseJsonParser::create_user(const std::string& username,
                                  const std::string& key,
                                  const std::string& user_id) {
  json j;
  j["method"] = "POST";
  j["path"] = "/user";
  j["name_table"] = "user";
  j["username"] = username;
  j["user_key"] = key;

  Array<string> values;
  values.push_back(user_id);
  values.push_back(username);
  values.push_back(key);

  string commands =
      "INSERT INTO user VALUES " + values.to_stringComand() + "\n";
  values.clear();

  string name_table = "lot";
  Table& currentTable = burse.replicaDB.searchTable(name_table);
  int index_lotID =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "lot_id");

  // TODO возможна ошибка когда не в 1 файле
  for (int i = 0; i < currentTable.csv[0].line.getSize(); i++) {
    string lot_id = currentTable.csv[0].line[i][index_lotID];

    values.push_back(user_id);
    values.push_back(lot_id);
    values.push_back("1000");

    commands +=
        "INSERT INTO user_lot VALUES " + values.to_stringComand() + "\n";

    values.clear();
  }
  j["command"] = commands;

  istringstream ss("lot user");
  string TableName;

  while (ss >> TableName) {
    burse.replicaDB.unloadSchemaData(TableName);
  }

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
  return j;
}

json BurseJsonParser::delete_order(const std::string& user_key, int order_id) {
  json j;
  return j;
}

json BurseJsonParser::get_lot() {
  string name_table = "lot";

  // Отправка запроса на сервер БД
  send_request_to_db(db_ip, db_port, name_table);

  // Поиск таблицы "lot" в БД
  Table& currentTable = burse.replicaDB.searchTable("lot");
  int index_lotId =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "lot_id");
  int index_name = burse.replicaDB.findColumnIndex(currentTable.csv[0], "name");

  // Массив для хранения данных о лотах
  json responseArray = json::array();

  // Проверяем, есть ли строки в таблице
  if (currentTable.csv.back().line.getSize() == 0) {
    // Если лотов нет, возвращаем ошибку
    json j;
    j["error"] = "лотов нет";
    return j;
  } else {
    // Проходим по строкам и формируем JSON
    for (int i = 0; i < currentTable.csv.back().line.getSize(); i++) {
      json lotJson;
      lotJson["lot_id"] =
          std::stoi(currentTable.csv.back().line[i][index_lotId]);
      lotJson["name"] =
          currentTable.csv.back().line[i][index_name];  // Имя из строки

      responseArray.push_back(lotJson);  // Добавляем лот в массив
    }
  }

  // Создаем итоговый объект с заголовком "lot"
  json finalResponse;
  finalResponse["lot"] = responseArray;

  finalResponse["method"] = "GET";
  finalResponse["path"] = "/lot";

  // Загружаем и очищаем схему данных
  string TableName;
  std::istringstream sss(name_table);
  while (sss >> TableName) {
    burse.replicaDB.unloadSchemaData(TableName);
  }

  // Выводим итоговый JSON (для отладки)
  cout << finalResponse << endl;

  return finalResponse;  // Возвращаем финальный JSON с заголовком "lot"
}

json BurseJsonParser::get_pair() {
  string name_table = "pair";

  // Отправка запроса на сервер БД
  send_request_to_db(db_ip, db_port, name_table);

  // Поиск таблицы "lot" в БД
  Table& currentTable = burse.replicaDB.searchTable("pair");
  int index_pairId =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "pair_id");
  int index_sale_lot_id =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "sale_lot_id");
  int index_buy_lot_id =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "buy_lot_id");

  // Массив для хранения данных о pair
  ordered_json responseArray = ordered_json::array();

  // Проверяем, есть ли строки в таблице
  if (currentTable.csv.back().line.getSize() == 0) {
    // Если лотов нет, возвращаем ошибку
    ordered_json j;
    j["error"] = "пар нет";
    return j;
  } else {
    // Проходим по строкам и формируем JSON
    for (int i = 0; i < currentTable.csv.back().line.getSize(); i++) {
      ordered_json lotJson;
      lotJson["pair_id"] =
          std::stoi(currentTable.csv.back().line[i][index_pairId]);
      lotJson["sale_lot_id"] =
          std::stoi(currentTable.csv.back().line[i][index_sale_lot_id]);
      lotJson["buy_lot_id"] =
          std::stoi(currentTable.csv.back().line[i][index_buy_lot_id]);

      responseArray.push_back(lotJson);  // Добавляем лот в массив
    }
  }

  // Создаем итоговый объект с заголовком "lot"
  ordered_json finalResponse;
  finalResponse["pair"] = responseArray;

  finalResponse["method"] = "GET";
  finalResponse["path"] = "/pair";

  // Загружаем и очищаем схему данных
  string TableName;
  std::istringstream sss(name_table);
  while (sss >> TableName) {
    burse.replicaDB.unloadSchemaData(TableName);
  }

  // Выводим итоговый JSON (для отладки)
  cout << finalResponse << endl;

  return finalResponse;  // "pair"
}

json BurseJsonParser::get_balance(const std::string& user_key) {
  string name_table = "user user_lot";

  // Отправка запроса на сервер БД
  send_request_to_db(db_ip, db_port, name_table);

  Table& currentTableUser = burse.replicaDB.searchTable("user");
  int index_key =
      burse.replicaDB.findColumnIndex(currentTableUser.csv[0], "key");
  int index_user_id =
      burse.replicaDB.findColumnIndex(currentTableUser.csv[0], "user_id");

  int user_id = -1;
  for (auto user : currentTableUser.csv.back().line) {
    if (user[index_key] == user_key) {
      user_id = std::stoi(user[index_user_id]);
    }
  }
  if (user_id == -1) {  // Если user_id не изменился
    json empty_response;
    empty_response["error"] = "пользователь не найден";
    return empty_response;
  }

  Table& currentTable = burse.replicaDB.searchTable("user_lot");
  int insex_user_id_lot =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "user_id");
  int index_lot_id =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "lot_id");
  int index_quantity =
      burse.replicaDB.findColumnIndex(currentTable.csv[0], "quantity");

  // Массив для хранения данных о pair
  ordered_json responseArray = ordered_json::array();

  // Проверяем, есть ли строки в таблице
  if (currentTable.csv.back().line.getSize() == 0) {
    // Если лотов нет, возвращаем ошибку
    ordered_json j;
    j["error"] = "пар нет";
    return j;
  } else {
    // Проходим по строкам и формируем JSON
    for (int i = 0; i < currentTable.csv.back().line.getSize(); i++) {
      if (std::stoi(currentTable.csv.back().line[i][insex_user_id_lot]) ==
          user_id) {
        ordered_json lotJson;
        lotJson["lot_id"] = currentTable.csv.back().line[i][index_lot_id];
        lotJson["quantity"] = currentTable.csv.back().line[i][index_quantity];

        responseArray.push_back(lotJson);  // Добавляем лот в массив
      } else {
        continue;
      }
    }
  }

  // Создаем итоговый объект с заголовком "lot"
  ordered_json finalResponse;
  finalResponse["balance"] = responseArray;

  finalResponse["method"] = "GET";
  finalResponse["path"] = "/balance";
  finalResponse["key"] = user_key;

  // Загружаем и очищаем схему данных
  string TableName;
  std::istringstream sss(name_table);
  while (sss >> TableName) {
    burse.replicaDB.unloadSchemaData(TableName);
  }

  // Выводим итоговый JSON (для отладки)
  cout << finalResponse << endl;

  return finalResponse;  // "pair"
}

json BurseJsonParser::set_configuration(const std::string& config_name) {
  json j;
  j["method"] = "POST";
  j["path"] = "/config";
  j["name_table"] = "lot";
  j["config_name"] = config_name;

  string commands = "";

  Array<string> values;
  for (int i = 0; i < burse.lots.getSize(); i++) {
    values.push_back(to_string(i + 1));
    values.push_back(burse.lots[i]);

    string comand = "INSERT INTO lot VALUES " + values.to_stringComand();
    commands += comand + "\n";
    values.clear();
  }
  int count_id = 1;
  for (int i = 0; i < burse.lots.getSize(); i++) {
    for (int j = 0; j < burse.lots.getSize(); j++) {
      if (i == j) {
        continue;
      }
      values.push_back(to_string(count_id++));
      values.push_back(to_string(i + 1));
      values.push_back(to_string(j + 1));

      string comand = "INSERT INTO pair VALUES " + values.to_stringComand();
      commands += comand + "\n";
      values.clear();
    }
  }

  values.clear();

  //"INSERT INTO lots VALUES('тест', 'тест', 'тест') ";
  j["command"] = commands;
  return j;
}

void BurseJsonParser::send_request_to_db(const std::string& db_ip, int db_port,
                                         std::string name_tables) {
  try {
    // Создаем IO контекст и сокет для соединения с сервером БД
    net::io_context ioc;
    tcp::socket socket(ioc);
    tcp::endpoint endpoint(net::ip::make_address(db_ip), db_port);

    // Подключаемся к серверу БД
    socket.connect(endpoint);

    // Создаем JSON-объект для запроса
    json request_json;
    request_json["method"] = "GET";  // Пример действия
    request_json["path"] = "/db";    // Пример параметра
    request_json["tables"] = name_tables;

    // Сериализуем JSON в строку
    std::string request_data = request_json.dump();

    // Отправляем запрос в виде строки
    boost::asio::write(socket, boost::asio::buffer(request_data));

    // Получаем ответ от БД
    char buffer[4096];
    size_t length = socket.read_some(boost::asio::buffer(buffer));

    // Преобразуем ответ в строку
    std::string response(buffer, length);

    // Парсим строку как JSON
    json response_json = json::parse(response);

    // С десериализацией данных от БД

    burse.replicaDB = burse.replicaDB.deserialize(response_json);

    // Закрываем сокет
    socket.close();

  } catch (const std::exception& e) {
    std::cerr << "Ошибка при соединении с БД: " << e.what() << std::endl;
  }
}
