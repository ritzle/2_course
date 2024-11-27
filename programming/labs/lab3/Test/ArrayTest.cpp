#include <iostream>

#include "../headers/Array.hpp"
#include "../lib/googletest/googlemock/include/gmock/gmock.h"
#include "../lib/googletest/googletest/include/gtest/gtest.h"

using namespace std;

// Фикстура
template <typename T>
class ArrayTest : public ::testing::Test {
 protected:
  Array<T> arr;

  void SetUp() override {}

  void TearDown() override {}
};

// Типы для тестирования
using MyTypes = ::testing::Types<int, string>;

TYPED_TEST_SUITE(ArrayTest, MyTypes);

TYPED_TEST(ArrayTest, TestEmplaceBack) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->arr.emplace_back(42);
    this->arr.emplace_back(99);
    EXPECT_EQ(this->arr[1], 99);

  } else if constexpr (is_same_v<TypeParam, string>) {
    this->arr.emplace_back(to_string(42));
    this->arr.emplace_back(to_string(99));
    EXPECT_EQ(this->arr[1], to_string(99));
  }
}

TYPED_TEST(ArrayTest, Insert) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->arr.insert(0, 42);
    this->arr.insert(0, 99);
    EXPECT_EQ(this->arr[0], 99);

  } else if constexpr (is_same_v<TypeParam, string>) {
    this->arr.insert(0, to_string(42));
    this->arr.insert(0, to_string(99));
    EXPECT_EQ(this->arr[0], to_string(99));
  }
}

TYPED_TEST(ArrayTest, Erase) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->arr.emplace_back(42);
    this->arr.emplace_back(99);

    this->arr.erase(0);

    EXPECT_EQ(this->arr[0], 99);

  } else if constexpr (is_same_v<TypeParam, string>) {
    this->arr.emplace_back(to_string(42));
    this->arr.emplace_back(to_string(99));

    this->arr.erase(0);

    EXPECT_EQ(this->arr[0], to_string(99));
  }
}

TYPED_TEST(ArrayTest, EraseByValue) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->arr.emplace_back(42);
    this->arr.emplace_back(99);

    this->arr.eraseByValue(42);

    EXPECT_EQ(this->arr[0], 99);

  } else if constexpr (is_same_v<TypeParam, string>) {
    this->arr.emplace_back(to_string(42));
    this->arr.emplace_back(to_string(99));

    this->arr.eraseByValue(to_string(42));

    EXPECT_EQ(this->arr[0], to_string(99));
  }
}

TYPED_TEST(ArrayTest, Replace) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->arr.emplace_back(42);
    this->arr.emplace_back(99);

    this->arr.replace(0, 99);

    EXPECT_EQ(this->arr[0], 99);

  } else if constexpr (is_same_v<TypeParam, string>) {
    this->arr.emplace_back(to_string(42));
    this->arr.emplace_back(to_string(99));

    this->arr.replace(0, to_string(99));

    EXPECT_EQ(this->arr[0], to_string(99));
  }
}

TYPED_TEST(ArrayTest, GetSize) {
  if constexpr (is_same_v<TypeParam, int>) {
    EXPECT_EQ(this->arr.getSize(), 0);

  } else if constexpr (is_same_v<TypeParam, string>) {
    EXPECT_EQ(this->arr.getSize(), 0);
  }
}

TYPED_TEST(ArrayTest, Print) {
  if constexpr (is_same_v<TypeParam, int>) {
    // Убедимся, что stdout захвачен
    testing::internal::CaptureStdout();

    this->arr.emplace_back(1);
    this->arr.emplace_back(2);
    this->arr.emplace_back(3);

    // Вызываем метод print
    this->arr.print();

    // Получаем захваченный вывод
    string output = testing::internal::GetCapturedStdout();

    // Сравниваем с ожидаемым значением
    EXPECT_EQ(output, "1 2 3 \n");

  } else if constexpr (is_same_v<TypeParam, string>) {
    // Убедимся, что stdout захвачен
    testing::internal::CaptureStdout();

    this->arr.emplace_back(to_string(1));
    this->arr.emplace_back(to_string(2));
    this->arr.emplace_back(to_string(3));

    // Вызываем метод print
    this->arr.print();

    // Получаем захваченный вывод
    string output = testing::internal::GetCapturedStdout();

    // Сравниваем с ожидаемым значением
    EXPECT_EQ(output, "1 2 3 \n");
  }
}

TYPED_TEST(ArrayTest, WriteToFile) {
  if constexpr (is_same_v<TypeParam, int>) {
    Array<int> arr1;

    arr1.emplace_back(1);
    arr1.emplace_back(2);
    arr1.emplace_back(3);
    arr1.emplace_back(4);
    arr1.emplace_back(99);

    arr1.writeToFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    this->arr.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    EXPECT_EQ(this->arr.getSize(), 5);
    EXPECT_EQ(this->arr[4], 99);

  } else if constexpr (is_same_v<TypeParam, string>) {
    Array<string> arr1;

    arr1.emplace_back("1");
    arr1.emplace_back("2");
    arr1.emplace_back("3");
    arr1.emplace_back("4");
    arr1.emplace_back("99");

    arr1.writeToFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    this->arr.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

    EXPECT_EQ(this->arr.getSize(), 5);
    EXPECT_EQ(this->arr[4], "99");
  }
}

TYPED_TEST(ArrayTest, fillFromFile) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->arr.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");

    EXPECT_EQ(this->arr.getSize(), 5);
    EXPECT_EQ(this->arr[0], 1);

  } else if constexpr (is_same_v<TypeParam, string>) {
    this->arr.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");

    EXPECT_EQ(this->arr.getSize(), 5);
    EXPECT_EQ(this->arr[0], "1");
  }
}

TYPED_TEST(ArrayTest, resize) {
  if constexpr (is_same_v<TypeParam, int>) {
    for (int i = 0; i <= 11; i++) {
      this->arr.emplace_back(i);
    }

  } else if constexpr (is_same_v<TypeParam, string>) {
    for (int i = 0; i <= 11; i++) {
      this->arr.emplace_back(to_string(i));
    }
  }

  EXPECT_EQ(this->arr.getSize(), 12);
}

TYPED_TEST(ArrayTest, throwTest) {
  EXPECT_THROW(this->arr[99], out_of_range);
  EXPECT_THROW(this->arr.erase(99), invalid_argument);

  if constexpr (is_same_v<TypeParam, int>) {
    EXPECT_THROW(this->arr.insert(99, 99), invalid_argument);
    EXPECT_THROW(this->arr.replace(99, 99), invalid_argument);

  } else if constexpr (is_same_v<TypeParam, string>) {
    EXPECT_THROW(this->arr.insert(99, "99"), invalid_argument);

    EXPECT_THROW(this->arr.replace(99, "99"), invalid_argument);
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
