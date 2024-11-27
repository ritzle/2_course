cmake_minimum_required(VERSION 3.10)

# Название проекта
project("gtest")

# Включаем поддержку тестов
enable_testing()

# Добавляем подкаталог с GoogleTest
add_subdirectory(../lib/googletest googletest_build)

# Добавляем флаги для покрытия кода (GCov или Clang)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

# Создаем исполнимые файлы,линкуем,создаем тесты
add_executable(ArrayTest_app ArrayTest.cpp)
target_link_libraries(ArrayTest_app gtest_main gmock_main)
add_test(NAME ArrayTest COMMAND ArrayTest_app)


add_executable(BinaryTreeTest_app BinaryTreeTest.cpp)
target_link_libraries(BinaryTreeTest_app gtest_main gmock_main)
add_test(NAME BinaryTreeTest COMMAND BinaryTreeTest_app)

add_executable(DListTest_app DListTest.cpp)
target_link_libraries(DListTest_app gtest_main gmock_main)
add_test(NAME DListTest COMMAND DListTest_app)

add_executable(OListTest_app OListTest.cpp)
target_link_libraries(OListTest_app gtest_main gmock_main)
add_test(NAME OListTest COMMAND OListTest_app)

add_executable(QueueTest_app QueueTest.cpp)
target_link_libraries(QueueTest_app gtest_main gmock_main)
add_test(NAME QueueTest COMMAND QueueTest_app)

add_executable(StackTest_app StackTest.cpp)
target_link_libraries(StackTest_app gtest_main gmock_main)
add_test(NAME StackTest COMMAND StackTest_app)

# Настроим сбор данных покрытия
add_custom_target(
    coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --coverage --output-on-failure
    COMMAND lcov --capture --directory . --output-file coverage.info --include '*.cpp'
    COMMAND genhtml coverage.info --output-directory out --ignore-errors mismatch
    COMMENT "Running tests, collecting coverage, and generating HTML report"
    DEPENDS ArrayTest_app BinaryTreeTest_app DListTest_app OListTest_app QueueTest StackTest
)