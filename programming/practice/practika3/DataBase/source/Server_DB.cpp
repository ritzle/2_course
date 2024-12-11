#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#include "../headers/SYBD/ComandParser.hpp"
#include "../headers/SYBD/SYBD.hpp"
#include "../headers/SYBD/Table.hpp"
#include "../headers/array.hpp"
#include "../lib/json.hpp"

//---биржа
#include "../headers/SYBD/BurseJsonParser.hpp"

using namespace std;

const int PORT = 7437;
const char* SERVER_IP = "127.0.0.1";  // Установите ваш IP-адрес

DB dataBase;
// SQLParser parser(dataBase);
BurseJsonParser parser(dataBase);

void* handle_client(void* arg) {
  int new_socket = *((int*)arg);
  delete (int*)arg;

  cout << "Клиент подключен." << endl;

  char buffer[2048] = {0};
  int bytesReceived;

  while ((bytesReceived = read(new_socket, buffer, sizeof(buffer) - 1)) > 0) {
    string input(buffer, bytesReceived);
    cout << "Получен запрос: " << input << endl;

    cout << input << endl;
    parser.parse(input);

    string response = "Запрос выполнен.\n";
    send(new_socket, response.c_str(), response.size(), 0);
    cout << "Ответ отправлен клиенту." << endl;
  }

  if (bytesReceived < 0) {
    cerr << "Ошибка при чтении данных от клиента" << endl;
  }

  close(new_socket);
  cout << "Клиент отключен." << endl;
  return nullptr;
}

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cerr << "Ошибка создания сокета" << endl;
    return -1;
  }

  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) < 0) {
    cerr << "Ошибка установки параметров сокета" << endl;
    close(server_fd);
    return -1;
  }

  // Настраиваем сокет
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(SERVER_IP);
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    cerr << "Ошибка привязки сокета" << endl;
    close(server_fd);
    return -1;
  }

  // слушаем
  if (listen(server_fd, 3) < 0) {
    cerr << "Ошибка при прослушивании" << endl;
    close(server_fd);
    return -1;
  }

  dataBase.readingConfiguration("../schema.json");
  cout << "Сервер запущен и ждет соединения на IP " << SERVER_IP << " и порту "
       << PORT << "..." << endl;

  while (true) {
    new_socket =
        accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
      cerr << "Ошибка принятия соединения" << endl;
      continue;
    }

    // обработка для каждого клиента в отдельном потоке
    int* client_socket = new int(new_socket);
    pthread_t thread_id;
    if (pthread_create(&thread_id, nullptr, handle_client,
                       (void*)client_socket) < 0) {
      cerr << "Ошибка при создании потока" << endl;
      close(new_socket);
    } else {
    }
  }

  close(server_fd);
  return 0;
}
