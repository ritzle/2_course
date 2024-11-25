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

# Создаем исполнимые файлы для тестов и связываем их с библиотеками GoogleTest и GoogleMock
add_executable(ArrayTest_app ArrayTest.cpp)

# Линкуем с GoogleTest и GoogleMock (если используем GoogleMock, если нет, уберите gmock_main)
target_link_libraries(ArrayTest_app gtest_main gmock_main)

# Добавляем тесты
add_test(NAME ArrayTest COMMAND ArrayTest_app)

# Настроим сбор данных покрытия
# Запуск CTest с параметрами для сбора покрытия
add_custom_target(
    coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --coverage --output-on-failure
    DEPENDS ArrayTest_app 
)

# Генерация отчета о покрытии с помощью lcov
add_custom_target(
    gen_coverage_report
    COMMAND lcov --capture --directory . --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory out --ignore-errors mismatch
    DEPENDS coverage
    COMMENT "Generating coverage report"
)
