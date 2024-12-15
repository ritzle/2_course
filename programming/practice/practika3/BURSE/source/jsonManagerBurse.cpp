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

  cout << req << endl;

  // Проверяем наличие нужных заголовков
  std::string user_key;
  if (req.find("X-USER-KEY") != req.end()) {
    user_key = std::string(req["X-USER-KEY"]);
  }

  std::string username;
  if (req.find("X-USER-NAME") != req.end()) {
    username = std::string(req["X-USER-NAME"]);
  }

  // Создаем поток для чтения из тела запроса
  std::istringstream stream(req.body());

  if (method == "POST") {
    if (path == "/user") {
      // FIXME надо написать функцию обращения к бд для получения последнего id

      send_request_to_db(db_ip, db_port);

      burse.replicaDB.printInfo();
      cout << endl;

      string user_id = "0";  // на 1 больше чем последнее в бд
      string user_key = std::to_string(burse._hash(username + user_id));

      string name_table = "user";
      // burse.replicaDB.unloadSchemaData(name_table);

      burse.replicaDB.printInfo();
      cout << endl;
      return create_user(username, user_key, user_id);
    }

    if (path == "/order") {
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
                                  const std::string& key,
                                  const std::string& user_id) {
  json j;
  j["method"] = "POST";
  j["path"] = "/user";
  j["name_table"] = "user";
  j["username"] = username;
  j["key"] = key;

  Array<string> values;
  values.push_back(user_id);
  values.push_back(username);
  values.push_back(key);

  string commands = "INSERT INTO user VALUES " + values.to_stringComand();
  j["command"] = commands;

  values.clear();
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
  json j;
  // Пример данных для лотов
  j["lot_id"] = 5678;
  return j;
}

json BurseJsonParser::get_pair() {
  json j;
  return j;
}

json BurseJsonParser::get_balance(const std::string& user_key) {
  json j;
  return j;
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

  for (int i = 0; i < burse.lots.getSize(); i++) {
    for (int j = 0; j < burse.lots.getSize(); j++) {
      values.push_back(to_string(j + 1));
      values.push_back(burse.lots[i]);
      values.push_back(burse.lots[j]);

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

void BurseJsonParser::send_request_to_db(const std::string& db_ip,
                                         int db_port) {
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

    cout << response_json << endl;

    // С десериализацией данных от БД
    burse.replicaDB = burse.replicaDB.deserialize(response_json);

    // Закрываем сокет
    socket.close();

  } catch (const std::exception& e) {
    std::cerr << "Ошибка при соединении с БД: " << e.what() << std::endl;
  }
}
