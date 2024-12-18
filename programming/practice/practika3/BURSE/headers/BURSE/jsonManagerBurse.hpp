#ifndef BURSE_JSON_PARSER_HPP
#define BURSE_JSON_PARSER_HPP

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

#include "../../lib/json.hpp"
#include "../array.hpp"
#include "burse.hpp"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

class BurseJsonParser {
 public:
  BurseJsonParser(Burse& burse);
  ~BurseJsonParser();

  string db_ip;
  int db_port;

  // Метод для обработки строки запроса и вызова соответствующей функции
  json handle_request(
      const boost::beast::http::request<boost::beast::http::string_body>& req);

  void send_request_to_db(const std::string& db_ip, int db_port,
                          std::string tables);

 private:
  Burse& burse;

  // Методы для обработки каждого типа запроса
  json create_user(const std::string& username, const std::string& key,
                   const std::string& user_id);
  json create_order(int pair_id, int quantity, double price, std::string type,
                    std::string user_key, std::string closed);
  json get_order();
  json delete_order(const std::string& user_key, int order_id);
  json get_lot();
  json get_pair();
  json get_balance(const std::string& user_key);

  json set_configuration(const std::string& config_name);  // тестовая

  // вспомогательные

  int find_id_by_key(const std::string& user_key);
  Array<int> find_sale_buy_lot_id(int pair_id);
  bool is_purchase_possible_buy(int quantity, double price, int sale_lot_id,
                                int buy_lot_id, int userID);
  bool is_purchase_possible_sale(int quantity, double price, int sale_lot_id,
                                 int buy_lot_id, int userID);
};

#include "../../source/jsonManagerBurse.cpp"

#endif  // BURSE_JSON_PARSER_HPP
