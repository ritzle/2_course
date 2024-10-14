#include "../headers/Table.hpp"

namespace fs = filesystem;

// Конструктор по умолчанию
Table::Table()
    : tableName(""),
      csv{},
      pk_sequence(0),
      pathTable(),
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
