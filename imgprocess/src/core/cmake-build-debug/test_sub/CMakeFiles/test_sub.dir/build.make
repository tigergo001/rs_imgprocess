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
include test_sub/CMakeFiles/test_sub.dir/depend.make

# Include the progress variables for this target.
include test_sub/CMakeFiles/test_sub.dir/progress.make

# Include the compile flags for this target's objects.
include test_sub/CMakeFiles/test_sub.dir/flags.make

test_sub/CMakeFiles/test_sub.dir/test_sub.cpp.o: test_sub/CMakeFiles/test_sub.dir/flags.make
test_sub/CMakeFiles/test_sub.dir/test_sub.cpp.o: ../test_sub/test_sub.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test_sub/CMakeFiles/test_sub.dir/test_sub.cpp.o"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_sub.dir/test_sub.cpp.o -c /home/penglei/code/tmp/imgprocess/src/core/test_sub/test_sub.cpp

test_sub/CMakeFiles/test_sub.dir/test_sub.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_sub.dir/test_sub.cpp.i"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/penglei/code/tmp/imgprocess/src/core/test_sub/test_sub.cpp > CMakeFiles/test_sub.dir/test_sub.cpp.i

test_sub/CMakeFiles/test_sub.dir/test_sub.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_sub.dir/test_sub.cpp.s"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/penglei/code/tmp/imgprocess/src/core/test_sub/test_sub.cpp -o CMakeFiles/test_sub.dir/test_sub.cpp.s

# Object files for target test_sub
test_sub_OBJECTS = \
"CMakeFiles/test_sub.dir/test_sub.cpp.o"

# External object files for target test_sub
test_sub_EXTERNAL_OBJECTS =

test_sub/libtest_sub.a: test_sub/CMakeFiles/test_sub.dir/test_sub.cpp.o
test_sub/libtest_sub.a: test_sub/CMakeFiles/test_sub.dir/build.make
test_sub/libtest_sub.a: test_sub/CMakeFiles/test_sub.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtest_sub.a"
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub && $(CMAKE_COMMAND) -P CMakeFiles/test_sub.dir/cmake_clean_target.cmake
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_sub.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test_sub/CMakeFiles/test_sub.dir/build: test_sub/libtest_sub.a

.PHONY : test_sub/CMakeFiles/test_sub.dir/build

test_sub/CMakeFiles/test_sub.dir/clean:
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub && $(CMAKE_COMMAND) -P CMakeFiles/test_sub.dir/cmake_clean.cmake
.PHONY : test_sub/CMakeFiles/test_sub.dir/clean

test_sub/CMakeFiles/test_sub.dir/depend:
	cd /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/penglei/code/tmp/imgprocess/src/core /home/penglei/code/tmp/imgprocess/src/core/test_sub /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub /home/penglei/code/tmp/imgprocess/src/core/cmake-build-debug/test_sub/CMakeFiles/test_sub.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test_sub/CMakeFiles/test_sub.dir/depend

