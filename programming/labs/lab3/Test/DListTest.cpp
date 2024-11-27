#include <fstream>
#include <iostream>

#include "../headers/DList.h"
#include "../lib/googletest/googlemock/include/gmock/gmock.h"
#include "../lib/googletest/googletest/include/gtest/gtest.h"

using namespace std;

// Фикстура
template <typename T>
class DListTest : public ::testing::Test {
 protected:
  DList<T> list;

  void SetUp() override {}

  void TearDown() override {}
};

// Типы для тестирования
using MyTypes = ::testing::Types<int, string>;

TYPED_TEST_SUITE(DListTest, MyTypes);

// Тест на добавление элемента в конец списка
TYPED_TEST(DListTest, PushBack) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    EXPECT_EQ(this->list[1], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("42");
    this->list.push_back("99");
    EXPECT_EQ(this->list[1], "99");
  }
}

// Тест на добавление элемента в начало списка
TYPED_TEST(DListTest, PushFront) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_front(42);
    this->list.push_front(99);
    EXPECT_EQ(this->list[0], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_front("42");
    this->list.push_front("99");
    EXPECT_EQ(this->list[0], "99");
  }
}

// Тест на удаление элемента с начала списка
TYPED_TEST(DListTest, PopFront) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.pop_front();
    EXPECT_EQ(this->list[0], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("42");
    this->list.push_back("99");
    this->list.pop_front();
    EXPECT_EQ(this->list[0], "99");
  }
}

// Тест на удаление элемента с конца списка
TYPED_TEST(DListTest, PopBack) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.pop_back();
    EXPECT_EQ(this->list[0], 42);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("42");
    this->list.push_back("99");
    this->list.pop_back();
    EXPECT_EQ(this->list[0], "42");
  }
}

// Тест на удаление элемента по значению
TYPED_TEST(DListTest, PopValue) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    this->list.push_back(42);
    this->list.pop_value(42);
    EXPECT_EQ(this->list[0], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("42");
    this->list.push_back("99");
    this->list.push_back("42");
    this->list.pop_value("42");
    EXPECT_EQ(this->list[0], "99");
  }
}

// Тест на поиск элемента по значению
TYPED_TEST(DListTest, SearchByValue) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);

    testing::internal::CaptureStdout();
    this->list.search_by_value(99);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "index value 99: 1 \n");
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("42");
    this->list.push_back("99");

    testing::internal::CaptureStdout();
    this->list.search_by_value("99");
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "index value 99: 1 \n");
  }
}

// Тест на операцию с оператором []
TYPED_TEST(DListTest, AccessOperator) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(42);
    this->list.push_back(99);
    EXPECT_EQ(this->list[0], 42);
    EXPECT_EQ(this->list[1], 99);
    EXPECT_THROW(this->list[2], out_of_range);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("42");
    this->list.push_back("99");
    EXPECT_EQ(this->list[0], "42");
    EXPECT_EQ(this->list[1], "99");
    EXPECT_THROW(this->list[2], out_of_range);
  }
}

// Тест на печать списка
TYPED_TEST(DListTest, Print) {
  if constexpr (is_same_v<TypeParam, int>) {
    testing::internal::CaptureStdout();
    this->list.push_back(1);
    this->list.push_back(2);
    this->list.push_back(3);
    this->list.print();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 \n");
  } else if constexpr (is_same_v<TypeParam, string>) {
    testing::internal::CaptureStdout();
    this->list.push_back("1");
    this->list.push_back("2");
    this->list.push_back("3");
    this->list.print();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 \n");
  }
}

// Тест на загрузку списка из файла
TYPED_TEST(DListTest, FillFromFile) {
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

// Тест на запись списка в файл
TYPED_TEST(DListTest, WriteToFile) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->list.push_back(1);
    this->list.push_back(2);
    this->list.push_back(3);
    this->list.writeToFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    DList<int> newList;
    newList.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");
    EXPECT_EQ(newList.GetSize(), 3);
    EXPECT_EQ(newList[2], 3);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->list.push_back("1");
    this->list.push_back("2");
    this->list.push_back("3");
    this->list.writeToFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    DList<string> newList;
    newList.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");
    EXPECT_EQ(newList.GetSize(), 3);
    EXPECT_EQ(newList[2], "3");
  }
}

// Тест на выброс исключений при ошибках
TYPED_TEST(DListTest, ThrowTest) {
  EXPECT_THROW(this->list[99], out_of_range);
  EXPECT_THROW(this->list.pop_front(), invalid_argument);
  EXPECT_THROW(this->list.pop_back(), invalid_argument);

  if constexpr (is_same_v<TypeParam, int>) {
    EXPECT_THROW(this->list.pop_value(99), invalid_argument);

  } else if constexpr (is_same_v<TypeParam, string>) {
    EXPECT_THROW(this->list.pop_value("9"), invalid_argument);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
