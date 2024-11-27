#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "../headers/queue.h"

using namespace std;

// Фикстура для очереди
template <typename T>
class QueueTest : public ::testing::Test {
 protected:
  Queue<T> queue;

  void SetUp() override {}
  void TearDown() override {}
};

// Типы данных для тестирования
using MyTypes = ::testing::Types<int, string>;
TYPED_TEST_SUITE(QueueTest, MyTypes);

// Тест на добавление элементов в очередь
TYPED_TEST(QueueTest, TestPush) {
  if constexpr (std::is_same_v<TypeParam, int>) {
    // Для типа int
    this->queue.push(1);
    this->queue.push(2);
    this->queue.push(3);
    EXPECT_EQ(this->queue.GetSize(), 3);  // Проверка размера очереди для int
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    // Для типа string
    this->queue.push("apple");
    this->queue.push("banana");
    this->queue.push("cherry");
    EXPECT_EQ(this->queue.GetSize(), 3);  // Проверка размера очереди для string
  }
}

// Тест на удаление элемента из очереди
TYPED_TEST(QueueTest, TestPop) {
  if constexpr (std::is_same_v<TypeParam, int>) {
    // Для типа int
    this->queue.push(1);
    this->queue.push(2);
    this->queue.push(3);
    this->queue.pop();
    EXPECT_EQ(this->queue.GetSize(), 2);  // Проверка размера очереди
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    // Для типа string
    this->queue.push("apple");
    this->queue.push("banana");
    this->queue.push("cherry");
    this->queue.pop();
    EXPECT_EQ(this->queue.GetSize(), 2);  // Проверка размера очереди
  }
}

// Тест на печать очереди
TYPED_TEST(QueueTest, TestPrint) {
  testing::internal::CaptureStdout();  // Захватываем вывод

  if constexpr (std::is_same_v<TypeParam, int>) {
    // Для типа int
    this->queue.push(1);
    this->queue.push(2);
    this->queue.push(3);
    this->queue.print();  // Печатаем очередь

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output,
              "1 2 3 \n");  // Проверяем, что вывод совпадает с ожидаемым
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    // Для типа string
    this->queue.push("hello");
    this->queue.push("world");
    this->queue.push("test");
    this->queue.print();  // Печатаем очередь

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(
        output,
        "hello world test \n");  // Проверяем, что вывод совпадает с ожидаемым
  }
}

// Тест на загрузку данных из файла
TYPED_TEST(QueueTest, TestFillFromFile) {
  if constexpr (std::is_same_v<TypeParam, int>) {
    // Для типа int
    this->queue.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");
    EXPECT_EQ(this->queue.GetSize(), 5);  // Проверяем размер очереди
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->queue.fillFromFile(
        "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/test.txt");
    EXPECT_EQ(this->queue.GetSize(), 5);  // Проверяем размер очереди
  }
}

// Тест на запись в файл с учетом типа данных
TYPED_TEST(QueueTest, TestWriteToFileWithType) {
  // Очистка файла перед тестом (если файл существует)
  std::ofstream file(
      "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt",
      std::ios::trunc);
  file.close();  // Закрытие, чтобы подготовить файл к записи

  // Добавляем элементы в очередь, тип которых зависит от TypeParam
  if constexpr (std::is_integral_v<TypeParam>) {
    this->queue.push(1);
    this->queue.push(2);
    this->queue.push(3);
  } else if constexpr (std::is_floating_point_v<TypeParam>) {
    this->queue.push(1.1);
    this->queue.push(2.2);
    this->queue.push(3.3);
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    this->queue.push("one");
    this->queue.push("two");
    this->queue.push("three");
  }

  // Записываем содержимое очереди в файл
  this->queue.writeToFile(
      "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

  // Открываем файл для чтения и проверяем его содержимое
  std::ifstream outFile(
      "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/out.txt");

  // Проверяем, открылся ли файл
  ASSERT_TRUE(outFile.is_open()) << "Could not open the file for reading.";

  std::string line;
  std::getline(outFile, line);  // Чтение первой строки из файла

  // Проверка содержимого файла в зависимости от типа данных
  if constexpr (std::is_integral_v<TypeParam>) {
    EXPECT_EQ(line, "1 2 3 ")
        << "File content does not match expected output for integers.";
  } else if constexpr (std::is_floating_point_v<TypeParam>) {
    EXPECT_EQ(line, "1.1 2.2 3.3 ") << "File content does not match expected "
                                       "output for floating point numbers.";
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    EXPECT_EQ(line, "one two three ")
        << "File content does not match expected output for strings.";
  }

  outFile.close();  // Закрываем файл после проверки
}

// Тест на получение размера очереди
TYPED_TEST(QueueTest, TestGetSize) {
  EXPECT_EQ(this->queue.GetSize(), 0);  // Пустая очередь

  if constexpr (std::is_same_v<TypeParam, int>) {
    // Для типа int
    this->queue.push(1);
    this->queue.push(2);
    EXPECT_EQ(this->queue.GetSize(), 2);  // Очередь после добавления элементов
  } else if constexpr (std::is_same_v<TypeParam, std::string>) {
    // Для типа string
    this->queue.push("hello");
    this->queue.push("world");
    EXPECT_EQ(this->queue.GetSize(), 2);  // Очередь после добавления элементов
  } else {
    // Пример для типа double
    this->queue.push(1.1);
    this->queue.push(2.2);
    EXPECT_EQ(this->queue.GetSize(), 2);  // Очередь после добавления элементов
  }
}
