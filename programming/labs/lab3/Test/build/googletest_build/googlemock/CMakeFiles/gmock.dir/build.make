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
include googletest_build/googlemock/CMakeFiles/gmock.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include googletest_build/googlemock/CMakeFiles/gmock.dir/compiler_depend.make

# Include the progress variables for this target.
include googletest_build/googlemock/CMakeFiles/gmock.dir/progress.make

# Include the compile flags for this target's objects.
include googletest_build/googlemock/CMakeFiles/gmock.dir/flags.make

googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: googletest_build/googlemock/CMakeFiles/gmock.dir/flags.make
googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: /home/vlad/vsCode/2_course/programming/labs/lab3/lib/googletest/googlemock/src/gmock-all.cc
googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: googletest_build/googlemock/CMakeFiles/gmock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o"
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o -MF CMakeFiles/gmock.dir/src/gmock-all.cc.o.d -o CMakeFiles/gmock.dir/src/gmock-all.cc.o -c /home/vlad/vsCode/2_course/programming/labs/lab3/lib/googletest/googlemock/src/gmock-all.cc

googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/gmock.dir/src/gmock-all.cc.i"
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vlad/vsCode/2_course/programming/labs/lab3/lib/googletest/googlemock/src/gmock-all.cc > CMakeFiles/gmock.dir/src/gmock-all.cc.i

googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/gmock.dir/src/gmock-all.cc.s"
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vlad/vsCode/2_course/programming/labs/lab3/lib/googletest/googlemock/src/gmock-all.cc -o CMakeFiles/gmock.dir/src/gmock-all.cc.s

# Object files for target gmock
gmock_OBJECTS = \
"CMakeFiles/gmock.dir/src/gmock-all.cc.o"

# External object files for target gmock
gmock_EXTERNAL_OBJECTS =

lib/libgmock.a: googletest_build/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
lib/libgmock.a: googletest_build/googlemock/CMakeFiles/gmock.dir/build.make
lib/libgmock.a: googletest_build/googlemock/CMakeFiles/gmock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../lib/libgmock.a"
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock.dir/cmake_clean_target.cmake
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gmock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
googletest_build/googlemock/CMakeFiles/gmock.dir/build: lib/libgmock.a
.PHONY : googletest_build/googlemock/CMakeFiles/gmock.dir/build

googletest_build/googlemock/CMakeFiles/gmock.dir/clean:
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock.dir/cmake_clean.cmake
.PHONY : googletest_build/googlemock/CMakeFiles/gmock.dir/clean

googletest_build/googlemock/CMakeFiles/gmock.dir/depend:
	cd /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vlad/vsCode/2_course/programming/labs/lab3/Test /home/vlad/vsCode/2_course/programming/labs/lab3/lib/googletest/googlemock /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock /home/vlad/vsCode/2_course/programming/labs/lab3/Test/build/googletest_build/googlemock/CMakeFiles/gmock.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : googletest_build/googlemock/CMakeFiles/gmock.dir/depend

