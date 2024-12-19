#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "../lib/json.hpp"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

class HttpClient {
 public:
  HttpClient(asio::io_context& ioc, const std::string& host,
             const std::string& port)
      : resolver_(ioc), socket_(ioc) {
    auto const results = resolver_.resolve(host, port);
    asio::connect(socket_, results.begin(), results.end());
  }

  void send_request(const std::string& request_type, const std::string& target,
                    const std::string& body = "",
                    const std::string& user_key = "") {
    try {
      http::request<http::string_body> req;

      if (request_type == "GET") {
        req.method(http::verb::get);
      } else if (request_type == "POST") {
        req.method(http::verb::post);
      } else if (request_type == "DELETE") {
        req.method(http::verb::delete_);
      } else {
        std::cerr << "Invalid request type: " << request_type << std::endl;
        return;
      }

      req.target(target);
      req.version(11);
      req.set(http::field::host, "localhost");
      req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

      // Устанавливаем заголовки для POST /user или других запросов
      if (request_type == "POST" && target == "/user") {
        if (!user_key.empty()) {
          req.set("X-USER-NAME", user_key);  // Устанавливаем имя пользователя
        }
      } else if (!user_key.empty()) {
        req.set("X-USER-KEY", user_key);  // Устанавливаем ключ пользователя
      }

      if (request_type == "POST" || request_type == "DELETE") {
        req.set(http::field::content_type, "application/json");
        req.set("BODY", body);
        req.prepare_payload();
      }

      http::write(socket_, req);

      beast::flat_buffer buffer;
      http::response<http::string_body> res;
      http::read(socket_, buffer, res);

      nlohmann::json jsonResponse = nlohmann::json::parse(res.body());

      std::cout << "Response: " << jsonResponse.dump(4) << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Error sending request: " << e.what() << std::endl;
    }
  }

  ~HttpClient() { socket_.close(); }

 private:
  tcp::resolver resolver_;
  tcp::socket socket_;
};

int main() {
  try {
    asio::io_context ioc;
    HttpClient client(ioc, "127.0.0.1", "8080");

    std::string line, request_type, request_target, user_key, body;

    while (true) {
      std::cout << ": ";
      std::getline(std::cin, line);

      if (line == "exit") {
        break;
      }

      std::istringstream stream(line);
      stream >> request_type >> request_target;

      // Проверяем, относится ли запрос к одной из команд с 3 аргументами
      if ((request_type == "POST" && request_target == "/order") ||
          (request_type == "DELETE" && request_target == "/order") ||
          (request_type == "GET" && request_target == "/balance")) {
        if (!(stream >> user_key)) {
          std::cerr << "Error: Missing X-USER-KEY in the request." << std::endl;
          continue;
        }

        // Если это POST /order, запрашиваем тело запроса
        if (request_type == "POST" && request_target == "/order") {
          std::cout << "Enter body: ";
          std::getline(std::cin, body);

          // Проверка количества слов
          std::istringstream iss(body);
          int word_count = 0;
          std::string word;
          std::vector<std::string> words;

          while (iss >> word) {
            words.push_back(word);
            word_count++;
          }

          if (word_count != 4) {
            std::cout << "Error: Please enter exactly 4 words.\n";
            continue;
          }

          // Проверка поля type
          std::string type = words[3];  // 4-е слово — это поле type
          if (type != "buy" && type != "sale") {
            std::cout
                << "Error: The 'type' field must be either 'buy' or 'sale'.\n";
            continue;
          }
        } else if (request_type == "DELETE" && request_target == "/order") {
          std::cout << "Enter body (order_id): ";
          std::getline(std::cin, body);

          // Проверка количества слов
          std::istringstream iss(body);
          int word_count = 0;
          std::string word;
          std::vector<std::string> words;

          while (iss >> word) {
            words.push_back(word);
            word_count++;
          }

          if (word_count != 1) {
            std::cout << "Error: Please enter exactly 4 words.\n";
            continue;
          }

        } else {
          body = "";  // Для других запросов тело не нужно
        }
      } else if (request_type == "POST" && request_target == "/user") {
        // Обрабатываем запрос POST /user, ожидаем имя
        if (!(stream >> user_key)) {
          std::cerr << "Error: Missing name in the request." << std::endl;
          continue;
        }
        body = "";
      } else {
        user_key = "";  // У остальных запросов ключ/имя не используются
        body = "";      // И тело не используется
      }

      // Отправляем запрос
      client.send_request(request_type, request_target, body, user_key);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
