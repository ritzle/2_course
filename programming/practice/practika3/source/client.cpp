#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <sstream>
#include <string>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

class HttpClient {
 public:
  HttpClient(asio::io_context& ioc, const std::string& host,
             const std::string& port)
      : resolver_(ioc), socket_(ioc) {
    // Разрешаем адрес и порт
    auto const results = resolver_.resolve(host, port);
    // Подключаемся к серверу
    asio::connect(socket_, results.begin(), results.end());
  }

  // Функция для отправки GET-запроса
  void send_get_request(const std::string& target) {
    http::request<http::string_body> req{http::verb::get, target, 11};
    req.set(http::field::host, "localhost");
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Отправка GET запроса
    http::write(socket_, req);

    // Чтение ответа
    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(socket_, buffer, res);

    std::cout << "Response: " << res.body() << std::endl;
  }

  // Функция для отправки POST-запроса
  void send_post_request(const std::string& target, const std::string& body) {
    http::request<http::string_body> req{http::verb::post, target, 11};
    req.set(http::field::host, "localhost");
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.body() = body;
    req.prepare_payload();

    // Отправка POST запроса
    http::write(socket_, req);

    // Чтение ответа
    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(socket_, buffer, res);

    std::cout << "Response: " << res.body() << std::endl;
  }

 private:
  tcp::resolver resolver_;
  tcp::socket socket_;
};

int main() {
  try {
    asio::io_context ioc;
    HttpClient client(ioc, "127.0.0.1", "8080");

    std::string line, request_type, request_target, body;

    while (true) {
      std::cout
          << "Enter request (e.g., 'GET /lot' or 'POST /create_lot {\"lot\": "
             "\"BTC\", \"price\": \"50000 USD\"}') or 'exit' to quit: ";
      std::getline(std::cin, line);  // Чтение всей строки

      if (line == "exit") {
        break;
      }

      std::istringstream stream(line);
      stream >> request_type >> request_target;

      // Если POST запрос, то нужно прочитать тело запроса
      if (request_type == "POST") {
        std::getline(stream, body);  // Чтение тела запроса
        body = body.substr(1);  // Удаление пробела после пути
      }

      if (request_type == "GET") {
        client.send_get_request(request_target);
      } else if (request_type == "POST") {
        client.send_post_request(request_target, body);
      } else {
        std::cout << "Invalid request type. Please use 'GET' or 'POST'.\n";
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
