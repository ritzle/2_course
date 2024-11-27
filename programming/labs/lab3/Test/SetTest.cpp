#include <iostream>

#include "../headers/set.hpp"
#include "../lib/googletest/googlemock/include/gmock/gmock.h"
#include "../lib/googletest/googletest/include/gtest/gtest.h"

using namespace std;

// Фикстура для тестирования класса Set
template <typename T>
class SetTest : public ::testing::Test {
 protected:
  Set<T> set;

  void SetUp() override {}

  void TearDown() override {}
};

// Типы данных для тестирования
using MyTypes = ::testing::Types<int, string>;
TYPED_TEST_SUITE(SetTest, MyTypes);

// Тест добавления элемента (emplace_back)
TYPED_TEST(SetTest, TestEmplaceBack) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->set.emplace_back(42);
    this->set.emplace_back(99);
    EXPECT_EQ(this->set.getSize(), 2);
    EXPECT_EQ(this->set[1], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->set.emplace_back("test1");
    this->set.emplace_back("test2");
    EXPECT_EQ(this->set.getSize(), 2);
    EXPECT_EQ(this->set[1], "test2");
  }
}

// Тест удаления элемента по индексу
TYPED_TEST(SetTest, TestErase) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->set.emplace_back(1);
    this->set.emplace_back(2);
    this->set.erase(0);
    EXPECT_EQ(this->set.getSize(), 1);
    EXPECT_EQ(this->set[0], 2);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->set.emplace_back("one");
    this->set.emplace_back("two");
    this->set.erase(0);
    EXPECT_EQ(this->set.getSize(), 1);
    EXPECT_EQ(this->set[0], "two");
  }
}

// Тест удаления элемента по значению
TYPED_TEST(SetTest, TestEraseByValue) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->set.emplace_back(10);
    this->set.emplace_back(20);
    this->set.eraseByValue(10);
    EXPECT_EQ(this->set.getSize(), 1);
    EXPECT_EQ(this->set[0], 20);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->set.emplace_back("alpha");
    this->set.emplace_back("beta");
    this->set.eraseByValue("alpha");
    EXPECT_EQ(this->set.getSize(), 1);
    EXPECT_EQ(this->set[0], "beta");
  }
}

// Тест замены элемента
TYPED_TEST(SetTest, TestReplace) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->set.emplace_back(30);
    this->set.replace(0, 99);
    EXPECT_EQ(this->set[0], 99);
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->set.emplace_back("replace");
    this->set.replace(0, "new");
    EXPECT_EQ(this->set[0], "new");
  }
}

// Тест получения размера
TYPED_TEST(SetTest, TestGetSize) {
  EXPECT_EQ(this->set.getSize(), 0);
  this->set.emplace_back(TypeParam{});
  EXPECT_EQ(this->set.getSize(), 1);
}

// Тест contains
TYPED_TEST(SetTest, TestContains) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->set.emplace_back(42);
    EXPECT_TRUE(this->set.contains(42));
    EXPECT_FALSE(this->set.contains(99));
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->set.emplace_back("exists");
    EXPECT_TRUE(this->set.contains("exists"));
    EXPECT_FALSE(this->set.contains("missing"));
  }
}

// Тест resize
TYPED_TEST(SetTest, TestResize) {
  for (int i = 0; i < 15; ++i) {
    if constexpr (is_same_v<TypeParam, int>) {
      this->set.emplace_back(i);
    } else if constexpr (is_same_v<TypeParam, string>) {
      this->set.emplace_back(to_string(i));
    }
  }
  EXPECT_EQ(this->set.getSize(), 15);
}

// Тест исключений
TYPED_TEST(SetTest, TestExceptions) {
  EXPECT_THROW(this->set.erase(0), invalid_argument);
  EXPECT_THROW(this->set.replace(0, TypeParam{}), invalid_argument);

  if constexpr (is_same_v<TypeParam, int>) {
    EXPECT_THROW(this->set[-1], out_of_range);
  } else if constexpr (is_same_v<TypeParam, string>) {
    EXPECT_THROW(this->set[-1], out_of_range);
  }
}

// Основная функция
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
