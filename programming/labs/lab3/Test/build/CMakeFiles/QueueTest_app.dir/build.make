# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vlad/vsCode/2_course/programming/labs/lab3/Test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build

# Include any dependencies generated for this target.
include CMakeFiles/QueueTest_app.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/QueueTest_app.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/QueueTest_app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/QueueTest_app.dir/flags.make

CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o: CMakeFiles/QueueTest_app.dir/flags.make
CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o: /home/vlad/vsCode/2_course/programming/labs/lab3/Test/QueueTest.cpp
CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o: CMakeFiles/QueueTest_app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o -MF CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o.d -o CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o -c /home/vlad/vsCode/2_course/programming/labs/lab3/Test/QueueTest.cpp

CMakeFiles/QueueTest_app.dir/QueueTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/QueueTest_app.dir/QueueTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vlad/vsCode/2_course/programming/labs/lab3/Test/QueueTest.cpp > CMakeFiles/QueueTest_app.dir/QueueTest.cpp.i

CMakeFiles/QueueTest_app.dir/QueueTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/QueueTest_app.dir/QueueTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vlad/vsCode/2_course/programming/labs/lab3/Test/QueueTest.cpp -o CMakeFiles/QueueTest_app.dir/QueueTest.cpp.s

# Object files for target QueueTest_app
QueueTest_app_OBJECTS = \
"CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o"

# External object files for target QueueTest_app
QueueTest_app_EXTERNAL_OBJECTS =

QueueTest_app: CMakeFiles/QueueTest_app.dir/QueueTest.cpp.o
QueueTest_app: CMakeFiles/QueueTest_app.dir/build.make
QueueTest_app: lib/libgtest_main.a
QueueTest_app: lib/libgmock_main.a
QueueTest_app: lib/libgmock.a
QueueTest_app: lib/libgtest.a
QueueTest_app: CMakeFiles/QueueTest_app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable QueueTest_app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QueueTest_app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/QueueTest_app.dir/build: QueueTest_app
.PHONY : CMakeFiles/QueueTest_app.dir/build

CMakeFiles/QueueTest_app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/QueueTest_app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/QueueTest_app.dir/clean

CMakeFiles/QueueTest_app.dir/depend:
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vlad/vsCode/2_course/programming/labs/lab3/Test /home/vlad/vsCode/2_course/programming/labs/lab3/Test /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/CMakeFiles/QueueTest_app.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/QueueTest_app.dir/depend

