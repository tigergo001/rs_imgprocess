# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/38/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/38/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/penglei/code/tmp/imgprocess/src/core

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug

# Include any dependencies generated for this target.
include test_add/CMakeFiles/test_add.dir/depend.make

# Include the progress variables for this target.
include test_add/CMakeFiles/test_add.dir/progress.make

# Include the compile flags for this target's objects.
include test_add/CMakeFiles/test_add.dir/flags.make

test_add/CMakeFiles/test_add.dir/test_add.cpp.o: test_add/CMakeFiles/test_add.dir/flags.make
test_add/CMakeFiles/test_add.dir/test_add.cpp.o: ../test_add/test_add.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test_add/CMakeFiles/test_add.dir/test_add.cpp.o"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_add.dir/test_add.cpp.o -c /home/penglei/code/tmp/imgprocess/src/core/test_add/test_add.cpp

test_add/CMakeFiles/test_add.dir/test_add.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_add.dir/test_add.cpp.i"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/penglei/code/tmp/imgprocess/src/core/test_add/test_add.cpp > CMakeFiles/test_add.dir/test_add.cpp.i

test_add/CMakeFiles/test_add.dir/test_add.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_add.dir/test_add.cpp.s"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/penglei/code/tmp/imgprocess/src/core/test_add/test_add.cpp -o CMakeFiles/test_add.dir/test_add.cpp.s

# Object files for target test_add
test_add_OBJECTS = \
"CMakeFiles/test_add.dir/test_add.cpp.o"

# External object files for target test_add
test_add_EXTERNAL_OBJECTS =

test_add/libtest_add.a: test_add/CMakeFiles/test_add.dir/test_add.cpp.o
test_add/libtest_add.a: test_add/CMakeFiles/test_add.dir/build.make
test_add/libtest_add.a: test_add/CMakeFiles/test_add.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtest_add.a"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add && $(CMAKE_COMMAND) -P CMakeFiles/test_add.dir/cmake_clean_target.cmake
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_add.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test_add/CMakeFiles/test_add.dir/build: test_add/libtest_add.a

.PHONY : test_add/CMakeFiles/test_add.dir/build

test_add/CMakeFiles/test_add.dir/clean:
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add && $(CMAKE_COMMAND) -P CMakeFiles/test_add.dir/cmake_clean.cmake
.PHONY : test_add/CMakeFiles/test_add.dir/clean

test_add/CMakeFiles/test_add.dir/depend:
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/penglei/code/tmp/imgprocess/src/core /home/penglei/code/tmp/imgprocess/src/core/test_add /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_add/CMakeFiles/test_add.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test_add/CMakeFiles/test_add.dir/depend

