#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "../headers/OList.h"

using namespace std;

// Фикстура для тестов OList
template <typename T>
class OListTest : public ::testing::Test {
 protected:
  OList<T> list;

  void SetUp() override {}
  void TearDown() override {}
};

// Типы для тестирования
using MyTypes = ::testing::Types<int, string>;

TYPED_TEST_SUITE(OListTest, MyTypes);

// Тест на добавление элементов в конец списка (push_back)
TYPED_TEST(OListTest, TestPushBack) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    EXPECT_EQ(this->list[0], 42);
    EXPECT_EQ(this->list[1], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    EXPECT_EQ(this->list[0], "hello");
    EXPECT_EQ(this->list[1], "world");
  }
}

// Тест на добавление элемента в начало списка (push_front)
TYPED_TEST(OListTest, TestPushFront) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_front(42);
    this->list.push_front(99);
    EXPECT_EQ(this->list[0], 99);
    EXPECT_EQ(this->list[1], 42);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_front("hello");
    this->list.push_front("world");
    EXPECT_EQ(this->list[0], "world");
    EXPECT_EQ(this->list[1], "hello");
  }
}

// Тест на удаление первого элемента (pop_front)
TYPED_TEST(OListTest, TestPopFront) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.pop_front();
    EXPECT_EQ(this->list[0], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    this->list.pop_front();
    EXPECT_EQ(this->list[0], "world");
  }
}

// Тест на удаление последнего элемента (pop_back)
TYPED_TEST(OListTest, TestPopBack) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.pop_back();
    EXPECT_EQ(this->list[0], 42);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    this->list.pop_back();
    EXPECT_EQ(this->list[0], "hello");
  }
}

// Тест на удаление элемента по значению (pop_value)
TYPED_TEST(OListTest, TestPopValue) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.push_back(42);
    this->list.pop_value(42);
    EXPECT_EQ(this->list[0], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    this->list.push_back("hello");
    this->list.pop_value("hello");
    EXPECT_EQ(this->list[0], "world");
  }
}

// Тест на поиск элемента по значению (search_by_value)
TYPED_TEST(OListTest, TestSearchByValue) {
  testing::internal::CaptureStdout();

  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.search_by_value(42);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    this->list.search_by_value("hello");
  }

  string output = testing::internal::GetCapturedStdout();
  EXPECT_TRUE(output.find("index value") !=
              string::npos);  // Убедитесь, что вывод содержит "index value"
}

// Тест на доступ по индексу (operator[])
TYPED_TEST(OListTest, TestOperatorBracket) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    EXPECT_EQ(this->list[0], 42);
    EXPECT_EQ(this->list[1], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    EXPECT_EQ(this->list[0], "hello");
    EXPECT_EQ(this->list[1], "world");
  }
}

// Тест на исключение при выходе за пределы индекса (operator[])
TYPED_TEST(OListTest, TestOperatorBracketOutOfBounds) {
  EXPECT_THROW(this->list[99], out_of_range);
}

// Тест на заполнение списка из файла
TYPED_TEST(OListTest, TestFillFromFile) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");
    EXPECT_EQ(this->list.GetSize(), 5);
    EXPECT_EQ(this->list[0], 1);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");
    EXPECT_EQ(this->list.GetSize(), 5);
    EXPECT_EQ(this->list[0], "1");
  }
}

// Тест на запись в файл
TYPED_TEST(OListTest, TestWriteToFile) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.writeToFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    // Прочитаем данные из файла и убедимся, что они записаны правильно
    ifstream file(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");
    int value;
    file >> value;
    EXPECT_EQ(value, 42);
    file >> value;
    EXPECT_EQ(value, 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    this->list.writeToFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    ifstream file(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");
    string value;
    file >> value;
    EXPECT_EQ(value, "hello");
    file >> value;
    EXPECT_EQ(value, "world");
  }
}

TYPED_TEST(OListTest, TestPrint) {
  // Захватим вывод в консоль
  testing::internal::CaptureStdout();

  // Добавим несколько элементов в список
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.push_back(123);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("hello");
    this->list.push_back("world");
    this->list.push_back("test");
  }

  // Вызываем метод print
  this->list.print();

  // Получаем захваченный вывод
  string output = testing::internal::GetCapturedStdout();

  // Проверим, что вывод соответствует ожидаемому
  if constexpr (is_same_v<TypeParam, int>) {
    EXPECT_EQ(output, "42 99 123 \n");  // Проверка для целых чисел
  } else if constexpr (is_same_v<TypeParam, string>) {
    EXPECT_EQ(output, "hello world test \n");  // Проверка для строк
  }
}

TYPED_TEST(OListTest, throwTest) {
  EXPECT_THROW(this->list[99], out_of_range);
  EXPECT_THROW(this->list.pop_back(), invalid_argument);
  EXPECT_THROW(this->list.pop_front(), invalid_argument);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
