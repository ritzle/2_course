#include "../headers/csv.hpp"

// Конструктор по умолчанию
CSV::CSV() : csvName(""), columns(), line() {}

// Конструктор с параметрами
CSV::CSV(const string& name, const Array<string>& cols)
    : csvName(name), columns(cols), line() {}

CSV::CSV(const string& name) : csvName(name), columns(), line() {}

int CSV::countLine() { return line.getSize(); }
