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
include CMakeFiles/sobel_8x8.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sobel_8x8.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sobel_8x8.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sobel_8x8.dir/flags.make

CMakeFiles/sobel_8x8.dir/main.cpp.o: CMakeFiles/sobel_8x8.dir/flags.make
CMakeFiles/sobel_8x8.dir/main.cpp.o: /home/ma9232/TestCoyote/bfv_backend/main.cpp
CMakeFiles/sobel_8x8.dir/main.cpp.o: CMakeFiles/sobel_8x8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sobel_8x8.dir/main.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sobel_8x8.dir/main.cpp.o -MF CMakeFiles/sobel_8x8.dir/main.cpp.o.d -o CMakeFiles/sobel_8x8.dir/main.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/main.cpp

CMakeFiles/sobel_8x8.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sobel_8x8.dir/main.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/main.cpp > CMakeFiles/sobel_8x8.dir/main.cpp.i

CMakeFiles/sobel_8x8.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sobel_8x8.dir/main.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/main.cpp -o CMakeFiles/sobel_8x8.dir/main.cpp.s

CMakeFiles/sobel_8x8.dir/util.cpp.o: CMakeFiles/sobel_8x8.dir/flags.make
CMakeFiles/sobel_8x8.dir/util.cpp.o: /home/ma9232/TestCoyote/bfv_backend/util.cpp
CMakeFiles/sobel_8x8.dir/util.cpp.o: CMakeFiles/sobel_8x8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sobel_8x8.dir/util.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sobel_8x8.dir/util.cpp.o -MF CMakeFiles/sobel_8x8.dir/util.cpp.o.d -o CMakeFiles/sobel_8x8.dir/util.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/util.cpp

CMakeFiles/sobel_8x8.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sobel_8x8.dir/util.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/util.cpp > CMakeFiles/sobel_8x8.dir/util.cpp.i

CMakeFiles/sobel_8x8.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sobel_8x8.dir/util.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/util.cpp -o CMakeFiles/sobel_8x8.dir/util.cpp.s

CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o: CMakeFiles/sobel_8x8.dir/flags.make
CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o: /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/scalar.cpp
CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o: CMakeFiles/sobel_8x8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o -MF CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o.d -o CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/scalar.cpp

CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/scalar.cpp > CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.i

CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/scalar.cpp -o CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.s

CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o: CMakeFiles/sobel_8x8.dir/flags.make
CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o: /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/vector.cpp
CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o: CMakeFiles/sobel_8x8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o -MF CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o.d -o CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/vector.cpp

CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/vector.cpp > CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.i

CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/coyote_out/sobel_8x8/vector.cpp -o CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.s

# Object files for target sobel_8x8
sobel_8x8_OBJECTS = \
"CMakeFiles/sobel_8x8.dir/main.cpp.o" \
"CMakeFiles/sobel_8x8.dir/util.cpp.o" \
"CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o" \
"CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o"

# External object files for target sobel_8x8
sobel_8x8_EXTERNAL_OBJECTS =

sobel_8x8: CMakeFiles/sobel_8x8.dir/main.cpp.o
sobel_8x8: CMakeFiles/sobel_8x8.dir/util.cpp.o
sobel_8x8: CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/scalar.cpp.o
sobel_8x8: CMakeFiles/sobel_8x8.dir/coyote_out/sobel_8x8/vector.cpp.o
sobel_8x8: CMakeFiles/sobel_8x8.dir/build.make
sobel_8x8: /home/ma9232/.conda/envs/coyoteEnv/lib/libseal-3.7.a
sobel_8x8: CMakeFiles/sobel_8x8.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable sobel_8x8"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sobel_8x8.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sobel_8x8.dir/build: sobel_8x8
.PHONY : CMakeFiles/sobel_8x8.dir/build

CMakeFiles/sobel_8x8.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sobel_8x8.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sobel_8x8.dir/clean

CMakeFiles/sobel_8x8.dir/depend:
	cd /home/ma9232/TestCoyote/bfv_backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ma9232/TestCoyote/bfv_backend /home/ma9232/TestCoyote/bfv_backend /home/ma9232/TestCoyote/bfv_backend/build /home/ma9232/TestCoyote/bfv_backend/build /home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles/sobel_8x8.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/sobel_8x8.dir/depend

