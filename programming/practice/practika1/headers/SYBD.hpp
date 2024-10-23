#ifndef BD_HPP
#define BD_HPP

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>

#include "Table.hpp"
#include "array.hpp"

using namespace std;
namespace fs = filesystem;

class DB {
 private:
 public:
  string schemaName;
  int tuplesLimit;
  string pathSchema;
  Array<Table> structure;

  DB();
  ~DB(){};

  void readingConfiguration(string PathSchema);
  void createDirectoriesAndFiles();
  void printInfo() const;

  // FIXME Инсерт после удалние если удалился csv фаил и последний не заполнен
  // создает новый а не добовляет в старый

  void insertIntoTable(string TableName, Array<string> values);

  void applySelect(const Array<string>& tableNames,
                   const Array<string>& tableColumns);
  void applyWhereConditions(const Array<string>& tableNames,
                            const Array<string>& tableColumns,
                            const Array<Array<string>>& conditional);
  void applyDeleteConditions(const Array<string>& tableNames,
                             const Array<Array<string>>& conditions);

 private:
  Table& searchTable(const string& TableName);
  string trim(const string& str);

  void loadExistingSchemaData(Array<string>& tableNames);
  void loadExistingSchemaData(const Array<string>& tableNames);
  void loadExistingSchemaData(string& tableName);

  void unloadSchemaData(Array<string>& tableNames);
  void unloadSchemaData(const Array<string>& tableNames);
  void unloadSchemaData(string& tableNames);

  Array<string> parseCSVLine(const string& line);

  bool checkAndConditionsAcrossTables(const Array<string>& conditionGroup,
                                      const Array<string>& row,
                                      const string& currentTableName,
                                      int currentCSVIndex, int currentRowIndex);

  // для where
  void processTableWithConditions(const Array<string>& tableNames,
                                  const Array<Array<string>>& conditional,
                                  function<void(const Array<string>&)> action);

  // для delete
  void processTableWithConditions(const Array<string>& tableNames,
                                  const Array<Array<string>>& conditional,
                                  const function<void(CSV&, int)>& action);

  int findColumnIndex(const CSV& csv, const string& columnName);

  Array<string> crossJoin(Array<string>& first, Array<string>& second);

  // TODO надо ли все эти функции в класс Table перенести???
  // FIXME не совсем коректное название возможно, подрозумевается запись в
  // фаил
  void updatePkSeqence(Table& table);
  void updateCSVFile(Table& table);  // только для вставки
  void updateLock(Table& table);

  void moveLinesBetweenCSVs(Table& table);

  void rewriteAllCSVFiles(Table& table);
  void rewriteFil(Table& table, int numberCsv);  // для rewriteCSVFile

  void rewriteFil(string& fileName, const Array<string>& columns,
                  const Array<string>& row);
  void rewriteFil(string& fileName,
                  Array<Array<string>> row);  // просто для записи в фаил
};
#include "../source/SYBD.cpp"
#endif