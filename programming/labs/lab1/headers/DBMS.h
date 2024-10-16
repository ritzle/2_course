#ifndef DBMS_H
#define DBMS_H

#include <sstream>
#include <string>

#include "array.h"  // Включаем заголовочный файл класса Array

class DBMS {
 public:
  // Конструктор и деструктор
  DBMS() = default;
  ~DBMS() = default;

  // Метод для обработки команд с массивами
  void handle(const string& request);

 private:
  void handleArray(const std::string& fileName, const std::string& query);
  void handleList(const string& fileName, const string& query);
  void handleDoobleList(const string& fileName, const string& query);
  void handleQueue(const string& fileName, const string& query);
  void handleStack(const string& fileName, const string& query);
  void handleTree(const string& fileName, const string& query);
};
#include "../source/DBMS.cpp"
#endif  // DBMS_H
