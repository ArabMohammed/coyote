# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /home/ma9232/.conda/envs/coyoteEnv/bin/cmake

# The command to remove a file.
RM = /home/ma9232/.conda/envs/coyoteEnv/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ma9232/TestCoyote/bfv_backend

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ma9232/TestCoyote/bfv_backend/build

# Include any dependencies generated for this target.
include CMakeFiles/conv_5x3_fully.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/conv_5x3_fully.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/conv_5x3_fully.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/conv_5x3_fully.dir/flags.make

CMakeFiles/conv_5x3_fully.dir/main.cpp.o: CMakeFiles/conv_5x3_fully.dir/flags.make
CMakeFiles/conv_5x3_fully.dir/main.cpp.o: /home/ma9232/TestCoyote/bfv_backend/main.cpp
CMakeFiles/conv_5x3_fully.dir/main.cpp.o: CMakeFiles/conv_5x3_fully.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/conv_5x3_fully.dir/main.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/conv_5x3_fully.dir/main.cpp.o -MF CMakeFiles/conv_5x3_fully.dir/main.cpp.o.d -o CMakeFiles/conv_5x3_fully.dir/main.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/main.cpp

CMakeFiles/conv_5x3_fully.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/conv_5x3_fully.dir/main.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/main.cpp > CMakeFiles/conv_5x3_fully.dir/main.cpp.i

CMakeFiles/conv_5x3_fully.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/conv_5x3_fully.dir/main.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/main.cpp -o CMakeFiles/conv_5x3_fully.dir/main.cpp.s

CMakeFiles/conv_5x3_fully.dir/util.cpp.o: CMakeFiles/conv_5x3_fully.dir/flags.make
CMakeFiles/conv_5x3_fully.dir/util.cpp.o: /home/ma9232/TestCoyote/bfv_backend/util.cpp
CMakeFiles/conv_5x3_fully.dir/util.cpp.o: CMakeFiles/conv_5x3_fully.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/conv_5x3_fully.dir/util.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/conv_5x3_fully.dir/util.cpp.o -MF CMakeFiles/conv_5x3_fully.dir/util.cpp.o.d -o CMakeFiles/conv_5x3_fully.dir/util.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/util.cpp

CMakeFiles/conv_5x3_fully.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/conv_5x3_fully.dir/util.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/util.cpp > CMakeFiles/conv_5x3_fully.dir/util.cpp.i

CMakeFiles/conv_5x3_fully.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/conv_5x3_fully.dir/util.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/util.cpp -o CMakeFiles/conv_5x3_fully.dir/util.cpp.s

CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o: CMakeFiles/conv_5x3_fully.dir/flags.make
CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o: /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/scalar.cpp
CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o: CMakeFiles/conv_5x3_fully.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o -MF CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o.d -o CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/scalar.cpp

CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/scalar.cpp > CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.i

CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/scalar.cpp -o CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.s

CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o: CMakeFiles/conv_5x3_fully.dir/flags.make
CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o: /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/vector.cpp
CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o: CMakeFiles/conv_5x3_fully.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o -MF CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o.d -o CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/vector.cpp

CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/vector.cpp > CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.i

CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/coyote_out/conv_5x3_fully/vector.cpp -o CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.s

# Object files for target conv_5x3_fully
conv_5x3_fully_OBJECTS = \
"CMakeFiles/conv_5x3_fully.dir/main.cpp.o" \
"CMakeFiles/conv_5x3_fully.dir/util.cpp.o" \
"CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o" \
"CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o"

# External object files for target conv_5x3_fully
conv_5x3_fully_EXTERNAL_OBJECTS =

conv_5x3_fully: CMakeFiles/conv_5x3_fully.dir/main.cpp.o
conv_5x3_fully: CMakeFiles/conv_5x3_fully.dir/util.cpp.o
conv_5x3_fully: CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/scalar.cpp.o
conv_5x3_fully: CMakeFiles/conv_5x3_fully.dir/coyote_out/conv_5x3_fully/vector.cpp.o
conv_5x3_fully: CMakeFiles/conv_5x3_fully.dir/build.make
conv_5x3_fully: /home/ma9232/.conda/envs/coyoteEnv/lib/libseal-3.7.a
conv_5x3_fully: CMakeFiles/conv_5x3_fully.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable conv_5x3_fully"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/conv_5x3_fully.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/conv_5x3_fully.dir/build: conv_5x3_fully
.PHONY : CMakeFiles/conv_5x3_fully.dir/build

CMakeFiles/conv_5x3_fully.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/conv_5x3_fully.dir/cmake_clean.cmake
.PHONY : CMakeFiles/conv_5x3_fully.dir/clean

CMakeFiles/conv_5x3_fully.dir/depend:
	cd /home/ma9232/TestCoyote/bfv_backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ma9232/TestCoyote/bfv_backend /home/ma9232/TestCoyote/bfv_backend /home/ma9232/TestCoyote/bfv_backend/build /home/ma9232/TestCoyote/bfv_backend/build /home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles/conv_5x3_fully.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/conv_5x3_fully.dir/depend

