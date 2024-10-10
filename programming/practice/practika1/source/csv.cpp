#include "../headers/csv.hpp"

// Конструктор по умолчанию
CSV::CSV() : csvName(""), columns(), line() {}

// Конструктор с параметрами
CSV::CSV(const string& name, const Array<string>& cols)
    : csvName(name), columns(cols), line() {}

int CSV::countLine() { return line.getSize(); }
