#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "../headers/binaryTree.hpp"

using namespace std;

// Фикстура для тестирования BinaryTree
template <typename T>
class BinaryTreeTest : public ::testing::Test {
 protected:
  BinaryTree<T> tree;

  void SetUp() override {
    // Инициализация дерева перед каждым тестом
  }

  void TearDown() override {
    // Очистка после теста (если нужно)
  }
};

// Типы для тестирования
using MyTypes = ::testing::Types<int, string>;

TYPED_TEST_SUITE(BinaryTreeTest, MyTypes);

// Тест на вставку
TYPED_TEST(BinaryTreeTest, Insert) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->tree.insert(42);
    this->tree.insert(99);
    this->tree.insert(15);
    this->tree.insert(78);

    EXPECT_TRUE(
        this->tree.search(42));  // Проверка, что 42 присутствует в дереве
    EXPECT_TRUE(
        this->tree.search(99));  // Проверка, что 99 присутствует в дереве
    EXPECT_FALSE(
        this->tree.search(100));  // Проверка, что 100 отсутствует в дереве
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->tree.insert("apple");
    this->tree.insert("banana");
    this->tree.insert("cherry");

    EXPECT_TRUE(this->tree.search("apple"));
    EXPECT_TRUE(this->tree.search("banana"));
    EXPECT_FALSE(this->tree.search("orange"));
  }
}

// Тест на поиск элемента
TYPED_TEST(BinaryTreeTest, Search) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->tree.insert(42);
    this->tree.insert(99);
    this->tree.insert(15);

    EXPECT_TRUE(this->tree.search(42));
    EXPECT_FALSE(this->tree.search(100));  // Элемент 100 не должен быть найден
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->tree.insert("apple");
    this->tree.insert("banana");

    EXPECT_TRUE(this->tree.search("apple"));
    EXPECT_FALSE(this->tree.search("grape"));
  }
}

// Тест на обход в порядке инордер
TYPED_TEST(BinaryTreeTest, InorderTraversal) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->tree.insert(50);
    this->tree.insert(30);
    this->tree.insert(70);
    this->tree.insert(20);
    this->tree.insert(40);
    this->tree.insert(60);
    this->tree.insert(80);

    // Ожидаемый порядок обхода: 20 30 40 50 60 70 80
    testing::internal::CaptureStdout();
    this->tree.inorder();  // Предполагаем, что inorder выводит на экран
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "20 30 40 50 60 70 80 ");
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->tree.insert("dog");
    this->tree.insert("cat");
    this->tree.insert("elephant");

    testing::internal::CaptureStdout();
    this->tree.inorder();  // Печать элементов в алфавитном порядке
    string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "cat dog elephant ");
  }
}

// Тест на сбалансированность дерева
TYPED_TEST(BinaryTreeTest, IsBalanced) {
  if constexpr (is_same_v<TypeParam, int>) {
    this->tree.insert(50);
    this->tree.insert(30);
    this->tree.insert(70);

    EXPECT_TRUE(
        this->tree.isBalanced());  // Проверяем, что дерево сбалансировано

    // Добавим элементы, чтобы сделать дерево несбалансированным
    this->tree.insert(20);
    this->tree.insert(10);

    EXPECT_FALSE(
        this->tree.isBalanced());  // Дерево должно стать несбалансированным
  } else if constexpr (is_same_v<TypeParam, string>) {
    this->tree.insert("apple");
    this->tree.insert("banana");

    EXPECT_TRUE(this->tree.isBalanced());  // Проверяем сбалансированность
  }
}

// Тест на выброс исключения (например, при поиске по несуществующему индексу)
// TYPED_TEST(BinaryTreeTest, ThrowTest) {
//   if constexpr (is_same_v<TypeParam, int>) {
//     EXPECT_THROW(this->tree.search(100), out_of_range);

//   } else if constexpr (is_same_v<TypeParam, string>) {
//     EXPECT_THROW(this->tree.search("100"), out_of_range);
//   }
// }

// // Тест на сериализацию и десериализацию
// TYPED_TEST(BinaryTreeTest, SerializeDeserialize) {
//   const std::string filename =
//       "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/tree_test.bin";

//   // Сначала вставляем элементы в дерево
//   if constexpr (is_same_v<TypeParam, int>) {
//     this->tree.insert(50);
//     this->tree.insert(30);
//     this->tree.insert(70);
//     this->tree.insert(20);
//     this->tree.insert(40);
//     this->tree.insert(60);
//     this->tree.insert(80);
//   } else if constexpr (is_same_v<TypeParam, string>) {
//     this->tree.insert("dog");
//     this->tree.insert("cat");
//     this->tree.insert("elephant");
//   }

//   // Сериализация дерева в файл
//   EXPECT_NO_THROW(this->tree.serialize(filename));

//   // Создаем новый объект дерева для десериализации
//   BinaryTree<TypeParam> newTree;

//   // Десериализация дерева из файла
//   EXPECT_NO_THROW(newTree.deserialize(filename));

//   // Проверяем, что элементы дерева восстановлены
//   if constexpr (is_same_v<TypeParam, int>) {
//     // Проверяем, что элементы присутствуют в восстановленном дереве
//     EXPECT_TRUE(newTree.search(50));
//     EXPECT_TRUE(newTree.search(30));
//     EXPECT_TRUE(newTree.search(70));
//     EXPECT_TRUE(newTree.search(20));
//     EXPECT_TRUE(newTree.search(40));
//     EXPECT_TRUE(newTree.search(60));
//     EXPECT_TRUE(newTree.search(80));

//     // Проверяем, что отсутствуют несуществующие элементы
//     EXPECT_FALSE(newTree.search(100));
//   } else if constexpr (is_same_v<TypeParam, string>) {
//     // Проверяем, что строки присутствуют в восстановленном дереве
//     EXPECT_TRUE(newTree.search("dog"));
//     EXPECT_TRUE(newTree.search("cat"));
//     EXPECT_TRUE(newTree.search("elephant"));

//     // Проверяем, что отсутствуют несуществующие строки
//     EXPECT_FALSE(newTree.search("lion"));
//   }
// }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
