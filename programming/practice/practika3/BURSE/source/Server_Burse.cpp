#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../headers/BURSE/jsonManagerBurse.hpp"

using namespace std;

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

BurseJsonParser parser;

// Функция для отправки запроса к серверу БД
std::string send_request_to_db(const std::string& db_ip, int db_port,
                               const std::string& query) {
  try {
    // Создаем IO контекст и сокет для соединения с сервером БД
    net::io_context ioc;
    tcp::socket socket(ioc);
    tcp::endpoint endpoint(net::ip::make_address(db_ip), db_port);

    // Подключаемся к серверу БД
    socket.connect(endpoint);

    // Формируем запрос в формате JSON или другом формате
    json request_json = parser.handle_request(query);
    string request = request_json.dump();

    // Отправляем запрос серверу БД
    boost::asio::write(socket, boost::asio::buffer(request));

    // Получаем ответ от БД
    char buffer[1024];
    size_t length = socket.read_some(boost::asio::buffer(buffer));

    // Преобразуем ответ в строку и возвращаем
    return std::string(buffer, length);
  } catch (const std::exception& e) {
    std::cerr << "Ошибка при соединении с БД: " << e.what() << std::endl;
    return "Ошибка при соединении с БД";
  }
}

// Функция для обработки запроса клиента
void handle_request(tcp::socket& socket, const std::string& db_ip,
                    int db_port) {
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

      // Подключаемся к серверу БД и отправляем запрос
      std::string db_response =
          send_request_to_db(db_ip, db_port, std::string(req.target()));

      // Ответ от БД
      res.body() = db_response;

      res.prepare_payload();

      // Отправляем ответ клиенту
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

void do_session(tcp::socket socket, const std::string& db_ip, int db_port) {
  try {
    std::string client_ip = socket.remote_endpoint().address().to_string();
    std::cout << "Client connected: " << client_ip << std::endl;
    // Обработка одного клиента
    handle_request(socket, db_ip, db_port);
    std::cout << "Disconnected: " << client_ip << std::endl;

    // Закрытие соединения
    socket.shutdown(tcp::socket::shutdown_both);
  } catch (const std::exception& e) {
    std::cerr << "Ошибка при соединении с клиентом: " << e.what() << std::endl;
  }
}

void server_loop(const std::string& address, unsigned short port,
                 const std::string& db_ip, int db_port) {
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
      std::thread{do_session, std::move(socket), db_ip, db_port}.detach();
    }
  } catch (const std::exception& e) {
    std::cerr << "Ошибка в сервере: " << e.what() << std::endl;
  }
}

int main() {
  std::string address =
      "0.0.0.0";  // Локальный адрес Биржи --- когда нули слушает все
  unsigned short port = 8080;       // Порт Биржи
  std::string db_ip = "127.0.0.1";  // IP-адрес сервера БД
  int db_port = 7437;               // Порт сервера БД

  server_loop(address, port, db_ip, db_port);  // Запуск серверного цикла

  return 0;
}
