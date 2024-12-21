#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

// Функция для обработки запроса

void handle_request(tcp::socket& socket) {
  try {
    beast::flat_buffer buffer;

    while (true) {
      buffer.consume(buffer.size());  // Очищаем буфер для нового запроса

      // Чтение HTTP запроса
      http::request<http::string_body> req;
      beast::error_code ec;

      http::read(socket, buffer, req, ec);

      // Проверяем, не произошел ли конец потока
      if (ec == http::error::end_of_stream) {
        break;  // Выходим из цикла, если клиент закрыл соединение
      } else if (ec) {
        std::cerr << "Error reading request: " << ec.message() << std::endl;
        break;
      }

      // Формируем HTTP-ответ
      http::response<http::string_body> res{http::status::ok, req.version()};
      res.set(http::field::server, "Boost.Beast HTTP Server");
      res.set(http::field::content_type, "text/plain");

      // Логика для обработки разных маршрутов
      if (req.target() == "/lot") {
        res.body() = "This is the /lot response";
      } else if (req.target() == "/user") {
        res.body() = "This is the /user response";
      } else {
        res.body() = "Unknown route!";
      }

      res.prepare_payload();

      // Отправляем ответ
      http::write(socket, res, ec);

      if (ec) {
        std::cerr << "Error writing response: " << ec.message() << std::endl;
        break;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Ошибка при обработке запроса: " << e.what() << std::endl;
  }
}

void do_session(tcp::socket socket) {
  try {
    std::string client_ip = socket.remote_endpoint().address().to_string();
    std::cout << "Client connected: " << client_ip << std::endl;
    // Обработка одного клиента
    handle_request(socket);
    std::cout << "Disconnected: " << client_ip << std::endl;

    // Закрытие соединения
    socket.shutdown(tcp::socket::shutdown_both);
  } catch (const std::exception& e) {
    std::cerr << "Ошибка при соединении с клиентом: " << e.what() << std::endl;
  }
}

void server_loop(const std::string& address, unsigned short port) {
  try {
    net::io_context ioc;

    tcp::acceptor acceptor(ioc,
                           tcp::endpoint(net::ip::make_address(address), port));
    std::cout << "Сервер запущен на " << address << ":" << port << std::endl;

    while (true) {
      // Принятие соединения
      tcp::socket socket(ioc);
      acceptor.accept(socket);

      // Запуск потока для обработки соединения
      std::thread{do_session, std::move(socket)}.detach();
    }
  } catch (const std::exception& e) {
    std::cerr << "Ошибка в сервере: " << e.what() << std::endl;
  }
}

int main() {
  std::string address = "127.0.0.1";  // Локальный адрес
  unsigned short port = 8080;         // Порт сервера

  server_loop(address, port);

  return 0;
}
