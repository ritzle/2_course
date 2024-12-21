#include "../headers/SYBD/Table.hpp"
namespace fs = filesystem;

// Конструктор по умолчанию
Table::Table()
    : tableName(""),
      csv{},
      pk_sequence(0),
      pathTable(),
      baseColumns(),
      lock(0),
      countCSVFile(1),
      tuplesLimit(100) {}  // Используйте конструктор по умолчанию для Array

// FIXME доделать может быть
// Конструктор с параметрами
Table::Table(const string& name, const Array<string>& cols)
    : tableName(name), countCSVFile(1), csv(csv) {}

// Конструктор копирования
Table::Table(const Table& other)
    : tableName(other.tableName),
      csv(other.csv),
      lock(other.lock),
      baseColumns(other.baseColumns),
      pathTable(other.pathTable),
      pk_sequence(other.pk_sequence),
      countCSVFile(other.countCSVFile),
      tuplesLimit(other.tuplesLimit) {}

// Оператор присваивания
Table& Table::operator=(const Table& other) {
  if (this != &other) {  // Проверка на самоприсваивание
    tableName = other.tableName;
    csv = other.csv;
    lock = other.lock;
    baseColumns = other.baseColumns;
    pathTable = other.pathTable;
    pk_sequence = other.pk_sequence;
    tuplesLimit = other.tuplesLimit;
    countCSVFile = other.countCSVFile;
  }
  return *this;
}

// Подсчет количества CSV-файлов
void Table::countCSVFiles() {
  countCSVFile = 0;  // Инициализируем счетчик
  for (const auto& entry : fs::directory_iterator(pathTable + "/")) {
    if (entry.path().extension() == ".csv") {
      countCSVFile++;  // Увеличиваем счетчик для каждого найденного CSV
    }
  }
}

// Чтение файла блокировки
void Table::readLockFile() {
  string lockFilePath = pathTable + "/" + tableName + "_lock";
  ifstream lockFile(lockFilePath);

  if (lockFile.is_open()) {
    lockFile >> lock;
    lockFile.close();
  } else {
    throw runtime_error("Не удалось открыть файл блокировки: " + lockFilePath);
  }
}

// Чтение файла последовательности pk_sequence
void Table::readPKSequenceFile() {
  string pkSequenceFilePath = pathTable + "/" + tableName + "_pk_seqence";
  ifstream pkFile(pkSequenceFilePath);

  if (pkFile.is_open()) {
    pkFile >>
        pk_sequence;  // Читаем значение последовательности первичного ключа
    pkFile.close();
  } else {
    throw runtime_error("Не удалось открыть файл последовательности PK: " +
                        pkSequenceFilePath);
  }
}

int Table::counterAllLine() {
  int totalLines = 0;

  for (auto csv : csv) {
    totalLines += csv.line.getSize();
  }
  return totalLines;
}

// Определение всех методов класса Table...

Array<string> Table::getColumnValues(const string& columnName) {
  Array<string> columnValues;

  // Предполагаем, что первый CSV содержит заголовки колонок
  if (csv.getSize() == 0) {
    cerr << "Ошибка: таблица " << tableName << " пуста." << endl;
    return columnValues;  // Возвращаем пустой массив
  }

  const CSV& firstCSV = csv[0];

  // Найти индекс колонки по имени
  int columnIndex = findColumnIndex(firstCSV, columnName);
  if (columnIndex == -1) {
    cerr << "Ошибка: колонка " << columnName << " не найдена." << endl;
    return columnValues;  // Возвращаем пустой массив
  }

  // Собираем значения колонки из всех CSV
  for (const CSV& currentCSV : csv) {
    for (const Array<string>& row : currentCSV.line) {
      if (row.getSize() > columnIndex) {
        columnValues.push_back(row[columnIndex]);
      } else {
        cerr << "Ошибка: строка содержит меньше колонок, чем ожидается."
             << endl;
      }
    }
  }

  return columnValues;
}

int Table::findColumnIndex(const CSV& csv, const string& columnName) const {
  for (int i = 0; i < csv.columns.getSize(); ++i) {
    if (csv.columns[i] == columnName) {
      return i;
    }
  }
  return -1;  // Если колонка не найдена
}

// Сериализация объекта Table в JSON
json Table::serialize() const {
  json j;

  // Добавляем поля объекта Table
  j["tableName"] = tableName;
  j["pathTable"] = pathTable;
  j["baseColumns"] =
      baseColumns.to_stringJson();  // Преобразуем baseColumns в строку JSON
  j["tuplesLimit"] = tuplesLimit;
  j["countCSVFile"] = countCSVFile;
  j["pk_sequence"] = pk_sequence;
  j["lock"] = lock;

  // Сериализация csv, т.к. у нас Array<CSV>, мы сериализуем каждый CSV
  json csvArray = json::array();
  for (const auto& c : csv) {
    csvArray.push_back(c.serialize());  // Сериализация каждого CSV
  }
  j["csv"] = csvArray;

  return j;
}

// Десериализация объекта Table из JSON
Table Table::deserialize(const json& j) {
  Table table;

  // Проверка наличия ключей в JSON
  if (j.contains("tableName")) {
    table.tableName = j["tableName"];
  }
  if (j.contains("pathTable")) {
    table.pathTable = j["pathTable"];
  }
  if (j.contains("tuplesLimit")) {
    table.tuplesLimit = j["tuplesLimit"];
  }
  if (j.contains("countCSVFile")) {
    table.countCSVFile = j["countCSVFile"];
  }
  if (j.contains("pk_sequence")) {
    table.pk_sequence = j["pk_sequence"];
  }
  if (j.contains("lock")) {
    table.lock = j["lock"];
  }

  // Десериализация baseColumns (используем метод из Array)
  if (j.contains("baseColumns") && j["baseColumns"].is_string()) {
    Array<std::string> baseColumns;
    baseColumns.from_stringJson(j["baseColumns"].get<std::string>());
    table.baseColumns = baseColumns;
  }

  // Десериализация csv (Array<CSV>)
  if (j.contains("csv") && j["csv"].is_array()) {
    for (const auto& csvJson : j["csv"]) {
      CSV c;
      c = c.deserialize(csvJson);  // Десериализация каждого CSV
      table.csv.push_back(c);  // Добавляем в таблицу
    }
  }

  return table;
}