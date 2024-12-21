#include "Http.h"

bool registerUser = false;

class Request {
 private:
  static bool isValidNumber(const string& str) {
    regex pattern(R"(^\d+(\.\d+)?$)");
    return regex_match(str, pattern);
  }

 public:
  static json PreparingRequestAndReadingResponsePOST(string& path,
                                                     beast::tcp_stream& stream,
                                                     nlohmann::json& request) {
    // Создание POST-запроса
    http::request<http::string_body> req(http::verb::post, path, 11);
    req.set(http::field::host, "127.0.0.1");
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::connection, "keep-alive");  // Запрос на Keep-Alive
    req.set(http::field::content_type,
            "application/json");  // Устанавливаем Content-Type
    string body_str;

    if (request.find("key") != request.end()) {
      req.set("X-USER-KEY", request["key"]);

      string pair_id = request["pair_id"].get<string>();
      string quantity = request["quantity"].get<string>();
      string price = request["price"].get<string>();
      string type = request["type"].get<string>();

      // Объединяем значения в итоговую строку
      body_str = pair_id + " " + quantity + " " + price + " " + type;
    }

    if (request.find("username") != request.end()) {
      req.set("X-USER-NAME", request["username"]);
      body_str = "";
    }
    req.set("BODY", body_str);
    req.body() = body_str;  // Тело запроса
    req.prepare_payload();  // Подготовка тела запроса

    // Отправка запроса
    http::write(stream, req);

    // Получение ответа
    beast::flat_buffer buffer;
    http::response<http::string_body> res;

    // Чтение ответа с обработкой ошибок
    try {
      http::read(stream, buffer, res);
    } catch (const boost::system::system_error& e) {
      cerr << "Ошибка чтения: " << e.what() << endl;
      throw runtime_error("Ошибка при чтении данных.");
    }

    // Парсинг JSON-ответа
    auto json_response = nlohmann::json::parse(res.body());

    // Вывод ответа
    cout << "Response: " << json_response.dump(4)
         << endl;  // Форматированный вывод JSON

    return json_response;
  }
  static json PreparingRequestAndReadingResponseGET(string& path,
                                                    beast::tcp_stream& stream) {
    http::request<http::string_body> req(http::verb::get, path, 11);
    req.set(http::field::host, "127.0.0.1");
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::connection, "keep-alive");  // Запрос на Keep-Alive
    req.set(http::field::content_type,
            "application/json");  // Устанавливаем Content-Type

    req.prepare_payload();  // Подготовка тела запроса

    // Отправка запроса
    http::write(stream, req);

    // Получение ответа
    beast::flat_buffer buffer;
    http::response<http::string_body> res;

    // Чтение ответа с обработкой ошибок
    try {
      http::read(stream, buffer, res);
    } catch (const boost::system::system_error& e) {
      cerr << "Ошибка чтения: " << e.what() << endl;
      throw runtime_error("Ошибка при чтении данных.");
    }

    // Парсинг JSON-ответа
    auto json_response = nlohmann::json::parse(res.body());

    // Вывод ответа
    cout << "Response: " << json_response.dump(4)
         << endl;  // Форматированный вывод JSON

    return json_response;
  }
  struct Post {
    static void Process_POST_user(string& path, beast::tcp_stream& stream) {
      if (registerUser == true) {
        cout << "Пользователь уже создан. Нельзя создать нового пользователя"
             << endl;
        return;
      } else if (registerUser == false) {
        registerUser = true;
      }

      string name;
      // Создание JSON для запроса /user
      nlohmann::json user_json;
      cout << "Введите имя пользователя: ";

      cin >> name;

      user_json["username"] = name;

      PreparingRequestAndReadingResponsePOST(path, stream, user_json);
    }

    static void Process_POST_order(string& path, beast::tcp_stream& stream) {
      if (registerUser == true) {
        cout << "Происходили запросы от другого ключа пользователя. Нельзя "
                "использовать другой ключ"
             << endl;
        return;
      } else if (registerUser == false) {
        registerUser = true;
      }
      // Создание JSON для запроса /order
      nlohmann::json order_json;
      string user_key, pair_id, quantity, price, type;
      cout << "Введите X-USER-KEY: ";
      cin >> user_key;
      cout << endl;
      cout << "Введите pair_id: ";
      cin >> pair_id;

      do {
        cout << "Введите quantity: ";
        cin >> quantity;
        if (!isValidNumber(quantity)) {
          cout << "Ошибка: quantity должно быть положительным целым или "
                  "дробным числом."
               << endl;
        }
      } while (!isValidNumber(quantity));
      cout << endl;

      do {
        cout << "Введите price: ";
        cin >> price;
        if (!isValidNumber(price)) {
          cout << "Ошибка: price должно быть положительным целым или дробным "
                  "числом."
               << endl;
        }
      } while (!isValidNumber(price));
      cout << endl;

      cout << "type: ";
      cin >> type;
      cout << endl;

      quantity += "0000000000";
      price += "0000000000";

      order_json["key"] = user_key;
      order_json["pair_id"] = pair_id;  // Пример, нужно ввести свои значения
      order_json["quantity"] = quantity;
      order_json["price"] = price;
      order_json["type"] = type;

      PreparingRequestAndReadingResponsePOST(path, stream, order_json);
    }
  };
};

class BOT {
 private:
  struct Lot {
    string lot_id;
    string name;
  };

  struct Pair {
    string pair_id;
    string name_sale_lot;
    string name_buy_lot;
  };

  struct Order {
    string pair_id;
    string quantity;
    string price;
    string type;
    string closed;
  };

 public:
  static void GetLot(vector<Lot>& lot, beast::tcp_stream& stream) {
    string path = "/lot";
    nlohmann::json json_response =
        Request::PreparingRequestAndReadingResponseGET(path, stream);

    // Извлекаем массив lot из json_response
    auto lot_array = json_response["lot"];

    // Обрабатываем каждый элемент массива
    for (auto& i : lot_array) {
      Lot structLot;
      structLot.lot_id = to_string(i["lot_id"]);
      structLot.name = to_string(i["name"]);
      lot.push_back(structLot);
    }
  }

  static void GetValidPair(vector<Pair>& pair, vector<Lot>& vecLot, string lot,
                           beast::tcp_stream& stream) {
    string path = "/pair";
    nlohmann::json json_response =
        Request::PreparingRequestAndReadingResponseGET(path, stream);

    // Извлекаем массив lot из json_response
    auto pair_array = json_response["pair"];

    Pair structPair;
    for (auto i : pair_array) {
      structPair.pair_id = to_string(i["pair_id"]);
      structPair.name_sale_lot = i["sale_lot"];
      structPair.name_buy_lot = i["buy_lot"];
      if (lot == i["sale_lot"] || lot == i["buy_lot"]) {
        pair.push_back(structPair);
      }
    }
  }

  static string PostUser(beast::tcp_stream& stream) {
    string path = "/user";
    string nameBot = "BotBuyingOrder";

    nlohmann::json json_request;
    json_request["username"] = nameBot;

    nlohmann::json json_response =
        Request::PreparingRequestAndReadingResponsePOST(path, stream,
                                                        json_request);

    string bot_key = json_response["user_key"];

    return bot_key;
  }

  static vector<Order> GetValidOrder(vector<Pair>& pair, string lot,
                                     beast::tcp_stream& stream) {
    string path = "/order";
    Order structOrder;
    vector<Order> order;

    nlohmann::json json_response =
        Request::PreparingRequestAndReadingResponseGET(path, stream);

    for (auto i : json_response) {
      structOrder.pair_id = i["pair_id"];
      structOrder.quantity = i["quantity"];
      structOrder.price = i["price"];
      structOrder.type = i["type"];
      structOrder.closed = i["closed"];

      order.push_back(structOrder);
    }

    vector<Order> validOrder;

    for (auto i : order) {
      for (auto j : pair) {
        if (lot == j.name_buy_lot && i.pair_id == j.pair_id &&
            i.closed.empty() && i.type == "sell") {
          structOrder.pair_id = i.pair_id;
          structOrder.quantity = i.quantity;
          structOrder.price = i.price;
          structOrder.type = "buy";
          validOrder.push_back(structOrder);
        } else if (lot == j.name_sale_lot && i.pair_id == j.pair_id &&
                   i.closed.empty() && i.type == "buy") {
          structOrder.pair_id = i.pair_id;
          structOrder.quantity = i.quantity;
          structOrder.price = i.price;
          structOrder.type = "sell";
          validOrder.push_back(structOrder);
        }
      }
    }

    return validOrder;
  }

  static void Start_bot(beast::tcp_stream& stream) {
    string path = "/order";
    nlohmann::json json_request;
    vector<Lot> vecLot;
    vector<Pair> vecValidPair;
    vector<Order> validOrder;

    GetLot(vecLot, stream);
    GetValidPair(vecValidPair, vecLot, "RUB", stream);

    string key_bot = PostUser(stream);

    while (true) {
      validOrder.clear();
      validOrder = GetValidOrder(vecValidPair, "RUB", stream);
      for (auto i : validOrder) {
        json_request["key"] = key_bot;
        json_request["pair_id"] = i.pair_id;
        json_request["quantity"] = i.quantity;
        json_request["price"] = i.price;
        json_request["type"] = i.type;

        Request::PreparingRequestAndReadingResponsePOST(path, stream,
                                                        json_request);
      }

      this_thread::sleep_for(chrono::milliseconds(10000));
    }
  }
};

int main() {
  net::io_context ioc;
  tcp::resolver resolver(ioc);
  string input, name;

  // Создание потока
  beast::tcp_stream stream(ioc);

  // Разрешение адреса
  auto const results = resolver.resolve("127.0.0.1", "8080");

  // Подключение к серверу
  net::connect(stream.socket(), results.begin(), results.end());

  BOT::Start_bot(stream);

  // Закрытие соединения
  stream.socket().shutdown(tcp::socket::shutdown_both);
}