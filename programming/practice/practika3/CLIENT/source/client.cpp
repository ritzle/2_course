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
    try {
      http::request<http::string_body> req{http::verb::get, target, 11};
      req.set(http::field::host, "localhost");
      req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
      req.set("X-Request-Name", "GET");

      std::cout << "Sending GET request to: " << target << std::endl;

      // Отправка GET запроса
      http::write(socket_, req);

      // Чтение ответа
      beast::flat_buffer buffer;
      http::response<http::string_body> res;
      http::read(socket_, buffer, res);

      std::cout << "Response: " << res.body() << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Error sending GET request: " << e.what() << std::endl;
    }
  }

  void send_post_request(const std::string& target, const std::string& body) {
    try {
      // Убираем все лишние пробелы и символы новой строки из тела
      std::string cleaned_body = body;
      cleaned_body.erase(
          std::remove_if(cleaned_body.begin(), cleaned_body.end(),
                         [](unsigned char x) { return std::isspace(x); }),
          cleaned_body.end());

      // Создание POST запроса
      http::request<http::string_body> req{http::verb::post, target, 11};
      req.set(http::field::host, "localhost");
      req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
      req.set(http::field::content_type, "application/json");
      req.set("data",
              cleaned_body);  // Сохраняем очищенное тело в заголовке "data"
      req.prepare_payload();
      req.set("X-Request-Name", "POST");

      // Отправка POST запроса
      http::write(socket_, req);

      // Чтение ответа
      beast::flat_buffer buffer;
      http::response<http::string_body> res;
      http::read(socket_, buffer, res);

      std::cout << "Response: " << res.body() << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Error sending POST request: " << e.what() << std::endl;
    }
  }

  ~HttpClient() {
    // Закрытие сокета при разрушении объекта
    socket_.close();
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
      std::cout << ": ";
      std::getline(std::cin, line);  // Чтение всей строки

      if (line == "exit") {
        break;
      }

      std::istringstream stream(line);
      stream >> request_type >> request_target;

      // Обработка POST запроса с телом запроса
      if (request_type == "POST") {
        // Чтение тела запроса (оно идет после пробела, после target)
        std::getline(stream, body);  // Чтение тела запроса
        body = body.substr(1);  // Удаление пробела после пути

        // Отправляем POST-запрос с телом в заголовке "data"
        client.send_post_request(request_target, body);
      } else if (request_type == "GET") {
        client.send_get_request(request_target);
      } else {
        std::cout << "Invalid request type. Please use 'GET' or 'POST'.\n";
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
