# CMake generated Testfile for 
# Source directory: /home/vlad/vsCode/2_course/programming/labs/lab3/Test
# Build directory: /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ArrayTest "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/ArrayTest_app")
set_tests_properties(ArrayTest PROPERTIES  _BACKTRACE_TRIPLES "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;19;add_test;/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;0;")
add_test(BinaryTreeTest "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/BinaryTreeTest_app")
set_tests_properties(BinaryTreeTest PROPERTIES  _BACKTRACE_TRIPLES "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;23;add_test;/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;0;")
add_test(DListTest "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/DListTest_app")
set_tests_properties(DListTest PROPERTIES  _BACKTRACE_TRIPLES "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;27;add_test;/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;0;")
add_test(OListTest "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/OListTest_app")
set_tests_properties(OListTest PROPERTIES  _BACKTRACE_TRIPLES "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;31;add_test;/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;0;")
add_test(QueueTest "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/QueueTest_app")
set_tests_properties(QueueTest PROPERTIES  _BACKTRACE_TRIPLES "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;35;add_test;/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;0;")
add_test(StackTest "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/StackTest_app")
set_tests_properties(StackTest PROPERTIES  _BACKTRACE_TRIPLES "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;39;add_test;/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;0;")
add_test(SetTest "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/SetTest_app")
set_tests_properties(SetTest PROPERTIES  _BACKTRACE_TRIPLES "/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;43;add_test;/home/vlad/vsCode/2_course/programming/labs/lab3/Test/CMakeLists.txt;0;")
subdirs("googletest_build")
