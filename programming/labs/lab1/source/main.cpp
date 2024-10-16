#include <iostream>

#include "../headers/DBMS.h"
#include "../headers/array.h"

int main() {
  DBMS db;
  string query;
  while (1) {
    getline(cin, query);

    db.handle(query);
  }
}
