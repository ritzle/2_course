#ifndef BURSE_JSON_PARSER_HPP
#define BURSE_JSON_PARSER_HPP

#include <string>

#include "../../lib/json.hpp"
#include "burse.hpp"

using json = nlohmann::json;

class BurseJsonParser {
 public:
  BurseJsonParser();
  ~BurseJsonParser();

  // Метод для обработки строки запроса и вызова соответствующей функции
  static json handle_request(const std::string& request);

 private:
  // Методы для обработки каждого типа запроса
  static json create_user(const std::string& username, const std::string& key);
  static json create_order(const std::string& user_key, int pair_id,
                           float quantity, float price,
                           const std::string& type);
  static json get_order();
  static json delete_order(const std::string& user_key, int order_id);
  static json get_lot();
  static json get_pair();
  static json get_balance(const std::string& user_key);
  static json set_configuration(const std::string& config_name,
                                const std::string& config_value);
};

#include "../../source/jsonManagerBurse.cpp"

#endif  // BURSE_JSON_PARSER_HPP
