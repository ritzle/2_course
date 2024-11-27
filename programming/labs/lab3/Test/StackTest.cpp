#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "../headers/stack.h"

using namespace std;

// Фикстура для стека
template <typename T>
class StackTest : public ::testing::Test {
 protected:
  Stack<T> stack;

  void SetUp() override {}
  void TearDown() override {}
};

// Типы данных для тестирования
using MyTypes = ::testing::Types<int, string>;
TYPED_TEST_SUITE(StackTest, MyTypes);

// Тест на добавление элемента в стек (push)
TYPED_TEST(StackTest, TestPush) {
  if constexpr (std::is_same_v<TypeParam, int>) {
    this->stack.push(1);
    this->stack.push(2);
    this->stack.push(3);
    EXPECT_EQ(this->stack.GetSize(), 3);  // Проверка размера стека для int
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->stack.push("apple");
    this->stack.push("banana");
    this->stack.push("cherry");
    EXPECT_EQ(this->stack.GetSize(), 3);  // Проверка размера стека для string
  }
}

// Тест на удаление элемента из стека (pop)
TYPED_TEST(StackTest, TestPop) {
  if constexpr (std::is_same_v<TypeParam, int>) {
    this->stack.push(1);
    this->stack.push(2);
    this->stack.push(3);
    this->stack.pop();  // Удаляем верхний элемент
    EXPECT_EQ(this->stack.GetSize(), 2);  // Проверка размера стека после pop

  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->stack.push("apple");
    this->stack.push("banana");
    this->stack.push("cherry");
    this->stack.pop();  // Удаляем верхний элемент
    EXPECT_EQ(this->stack.GetSize(), 2);  // Проверка размера стека после pop
  }
}

// Тест на печать стека
TYPED_TEST(StackTest, TestPrint) {
  testing::internal::CaptureStdout();  // Захватываем вывод

  if constexpr (std::is_same_v<TypeParam, int>) {
    this->stack.push(1);
    this->stack.push(2);
    this->stack.push(3);
    this->stack.print();  // Печатаем стек
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 \n");  // Проверка вывода стека для int
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->stack.push("hello");
    this->stack.push("world");
    this->stack.push("test");
    this->stack.print();  // Печатаем стек
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output,
              "hello world test \n");  // Проверка вывода стека для string
  }
}

// Тест на загрузку данных из файла в стек
TYPED_TEST(StackTest, TestFillFromFile) {
  if constexpr (std::is_same_v<TypeParam, int>) {
    this->stack.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");  // Путь к файлу
    EXPECT_EQ(this->stack.GetSize(),
              5);  // Проверка размера стека после загрузки
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->stack.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");  // Путь к файлу
    EXPECT_EQ(this->stack.GetSize(),
              5);  // Проверка размера стека после загрузки
  }
}

// Тест на запись стека в файл
TYPED_TEST(StackTest, TestWriteToFileWithType) {
  std::ofstream file(
      "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt",
      std::ios::trunc);  // Очистка файла
  file.close();          // Закрытие файла

  if constexpr (std::is_integral_v<TypeParam>) {
    this->stack.push(1);
    this->stack.push(2);
    this->stack.push(3);
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->stack.push("one");
    this->stack.push("two");
    this->stack.push("three");
  }

  // Записываем содержимое стека в файл
  this->stack.writeToFile(
      "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

  // Проверяем содержимое файла
  std::ifstream outFile(
      "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");
  std::string line;
  std::getline(outFile, line);  // Чтение первой строки
  if constexpr (std::is_integral_v<TypeParam>) {
    EXPECT_EQ(line, "1 2 3 ")
        << "File content does not match expected output for integers.";
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    EXPECT_EQ(line, "one two three ")
        << "File content does not match expected output for strings.";
  }

  outFile.close();  // Закрываем файл
}

// Тест на получение размера стека
TYPED_TEST(StackTest, TestGetSize) {
  EXPECT_EQ(this->stack.GetSize(), 0);  // Пустой стек

  if constexpr (std::is_same_v<TypeParam, int>) {
    this->stack.push(1);
    this->stack.push(2);
    EXPECT_EQ(this->stack.GetSize(),
              2);  // Проверка размера стека после добавления элементов
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->stack.push("hello");
    this->stack.push("world");
    EXPECT_EQ(this->stack.GetSize(),
              2);  // Проверка размера стека после добавления элементов
  }
}
