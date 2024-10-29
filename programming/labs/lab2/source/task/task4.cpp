#include <iostream>
#include <string>

#include "../../headers/Array.hpp"
bool isMatch(string& str, string& pattern);

int main() {
  string str, pattern;

  while (1) {
    cout << "Ввидите патерн: ";
    getline(cin, pattern);

    cout << "Ввидите последовательность: ";
    getline(cin, str);

    if (isMatch(str, pattern)) {
      cout << str << " соответствует шаблону " << pattern;
    } else {
      cout << str << " не соответствует шаблону " << pattern;
    }

    cout << endl;
  }
}

bool isMatch(string& str, string& pattern) {
  int strIndex = 0, patternIndex = 0;
  int strLen = str.size();
  int patLen = pattern.size();

  int lastStarIdx = -1;
  int lastMatchIdx = 0;

  Array<int> matchHistory;

  while (strIndex < strLen) {
    // Если символы совпадают или шаблон содержит '?', сдвигаем оба указателя
    if (patternIndex < patLen && (pattern[patternIndex] == '?' ||
                                  pattern[patternIndex] == str[strIndex])) {
      ++strIndex;
      ++patternIndex;
    }

    // Если встречается '*', сохраняем его индекс и позицию в строке
    else if (patternIndex < patLen && pattern[patternIndex] == '*') {
      lastStarIdx = patternIndex;
      lastMatchIdx = strIndex;
      matchHistory.emplace_back(lastMatchIdx);  // Записываем индекс строки
      ++patternIndex;                           // Пропускаем '*'
    }

    // Если несоответствие, но был '*', пытаемся расширить '*'
    else if (lastStarIdx != -1) {
      patternIndex = lastStarIdx + 1;
      ++lastMatchIdx;
      strIndex = lastMatchIdx;
      matchHistory.emplace_back(lastMatchIdx);  // Обновляем индекс строки
    } else {
      return false;  // Если нет соответствия и нет '*', возвращаем false
    }
  }

  // Убедимся, что оставшиеся символы в шаблоне - только '*'
  while (patternIndex < patLen && pattern[patternIndex] == '*') {
    ++patternIndex;
  }

  return patternIndex == patLen;
}