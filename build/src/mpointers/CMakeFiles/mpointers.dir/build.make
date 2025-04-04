# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/thedegeo/Documents/T/MPointers2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thedegeo/Documents/T/MPointers2/build

# Include any dependencies generated for this target.
include src/mpointers/CMakeFiles/mpointers.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/mpointers/CMakeFiles/mpointers.dir/compiler_depend.make

# Include the progress variables for this target.
include src/mpointers/CMakeFiles/mpointers.dir/progress.make

# Include the compile flags for this target's objects.
include src/mpointers/CMakeFiles/mpointers.dir/flags.make

src/mpointers/CMakeFiles/mpointers.dir/codegen:
.PHONY : src/mpointers/CMakeFiles/mpointers.dir/codegen

src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.o: src/mpointers/CMakeFiles/mpointers.dir/flags.make
src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.o: /home/thedegeo/Documents/T/MPointers2/src/mpointers/mpointer.cpp
src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.o: src/mpointers/CMakeFiles/mpointers.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/thedegeo/Documents/T/MPointers2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.o"
	cd /home/thedegeo/Documents/T/MPointers2/build/src/mpointers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.o -MF CMakeFiles/mpointers.dir/mpointer.cpp.o.d -o CMakeFiles/mpointers.dir/mpointer.cpp.o -c /home/thedegeo/Documents/T/MPointers2/src/mpointers/mpointer.cpp

src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mpointers.dir/mpointer.cpp.i"
	cd /home/thedegeo/Documents/T/MPointers2/build/src/mpointers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thedegeo/Documents/T/MPointers2/src/mpointers/mpointer.cpp > CMakeFiles/mpointers.dir/mpointer.cpp.i

src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mpointers.dir/mpointer.cpp.s"
	cd /home/thedegeo/Documents/T/MPointers2/build/src/mpointers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thedegeo/Documents/T/MPointers2/src/mpointers/mpointer.cpp -o CMakeFiles/mpointers.dir/mpointer.cpp.s

# Object files for target mpointers
mpointers_OBJECTS = \
"CMakeFiles/mpointers.dir/mpointer.cpp.o"

# External object files for target mpointers
mpointers_EXTERNAL_OBJECTS =

src/mpointers/libmpointers.a: src/mpointers/CMakeFiles/mpointers.dir/mpointer.cpp.o
src/mpointers/libmpointers.a: src/mpointers/CMakeFiles/mpointers.dir/build.make
src/mpointers/libmpointers.a: src/mpointers/CMakeFiles/mpointers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/thedegeo/Documents/T/MPointers2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libmpointers.a"
	cd /home/thedegeo/Documents/T/MPointers2/build/src/mpointers && $(CMAKE_COMMAND) -P CMakeFiles/mpointers.dir/cmake_clean_target.cmake
	cd /home/thedegeo/Documents/T/MPointers2/build/src/mpointers && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mpointers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/mpointers/CMakeFiles/mpointers.dir/build: src/mpointers/libmpointers.a
.PHONY : src/mpointers/CMakeFiles/mpointers.dir/build

src/mpointers/CMakeFiles/mpointers.dir/clean:
	cd /home/thedegeo/Documents/T/MPointers2/build/src/mpointers && $(CMAKE_COMMAND) -P CMakeFiles/mpointers.dir/cmake_clean.cmake
.PHONY : src/mpointers/CMakeFiles/mpointers.dir/clean

src/mpointers/CMakeFiles/mpointers.dir/depend:
	cd /home/thedegeo/Documents/T/MPointers2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thedegeo/Documents/T/MPointers2 /home/thedegeo/Documents/T/MPointers2/src/mpointers /home/thedegeo/Documents/T/MPointers2/build /home/thedegeo/Documents/T/MPointers2/build/src/mpointers /home/thedegeo/Documents/T/MPointers2/build/src/mpointers/CMakeFiles/mpointers.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/mpointers/CMakeFiles/mpointers.dir/depend

