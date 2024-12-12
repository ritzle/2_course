#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

int main() {
  const string SERVER_IP = "127.0.0.1";  // IP-адрес сервера
  const int SERVER_PORT = 7437;          // Порт сервера
  int sock = 0;
  struct sockaddr_in serv_addr;

  // Создаем сокет
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Ошибка: Не удалось создать сокет" << endl;
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);

  // Преобразование IP-адреса из текста в двоичную форму
  if (inet_pton(AF_INET, SERVER_IP.c_str(), &serv_addr.sin_addr) <= 0) {
    cout << "Ошибка: Неправильный адрес/Адрес не поддерживается" << endl;
    return -1;
  }

  // Подключение к серверу
  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    cout << "Ошибка: Подключение не удалось" << endl;
    return -1;
  }

  cout << "Введите запрос или 'exit' для выхода: ";
  string query;
  while (getline(cin, query)) {
    if (query == "exit") {
      cout << "Программа завершена." << endl;
      break;
    }

    // Отправка запроса на сервер
    send(sock, query.c_str(), query.size(), 0);
    cout << "Запрос отправлен." << endl;

    // Получение ответа от сервера
    char buffer[1024] = {0};
    int bytesReceived = read(sock, buffer, 1024);
    if (bytesReceived > 0) {
      cout << "Ответ от сервера: " << string(buffer, bytesReceived) << endl;
    }

    cout << "Введите SQL-запрос или 'exit' для выхода: ";
  }

  // Закрываем сокет
  close(sock);
  return 0;
}
