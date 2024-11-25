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

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
