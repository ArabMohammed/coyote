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
include CMakeFiles/l2_distance_16.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/l2_distance_16.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/l2_distance_16.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/l2_distance_16.dir/flags.make

CMakeFiles/l2_distance_16.dir/main.cpp.o: CMakeFiles/l2_distance_16.dir/flags.make
CMakeFiles/l2_distance_16.dir/main.cpp.o: /home/ma9232/TestCoyote/bfv_backend/main.cpp
CMakeFiles/l2_distance_16.dir/main.cpp.o: CMakeFiles/l2_distance_16.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/l2_distance_16.dir/main.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/l2_distance_16.dir/main.cpp.o -MF CMakeFiles/l2_distance_16.dir/main.cpp.o.d -o CMakeFiles/l2_distance_16.dir/main.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/main.cpp

CMakeFiles/l2_distance_16.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/l2_distance_16.dir/main.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/main.cpp > CMakeFiles/l2_distance_16.dir/main.cpp.i

CMakeFiles/l2_distance_16.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/l2_distance_16.dir/main.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/main.cpp -o CMakeFiles/l2_distance_16.dir/main.cpp.s

CMakeFiles/l2_distance_16.dir/util.cpp.o: CMakeFiles/l2_distance_16.dir/flags.make
CMakeFiles/l2_distance_16.dir/util.cpp.o: /home/ma9232/TestCoyote/bfv_backend/util.cpp
CMakeFiles/l2_distance_16.dir/util.cpp.o: CMakeFiles/l2_distance_16.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/l2_distance_16.dir/util.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/l2_distance_16.dir/util.cpp.o -MF CMakeFiles/l2_distance_16.dir/util.cpp.o.d -o CMakeFiles/l2_distance_16.dir/util.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/util.cpp

CMakeFiles/l2_distance_16.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/l2_distance_16.dir/util.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/util.cpp > CMakeFiles/l2_distance_16.dir/util.cpp.i

CMakeFiles/l2_distance_16.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/l2_distance_16.dir/util.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/util.cpp -o CMakeFiles/l2_distance_16.dir/util.cpp.s

CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o: CMakeFiles/l2_distance_16.dir/flags.make
CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o: /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/scalar.cpp
CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o: CMakeFiles/l2_distance_16.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o -MF CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o.d -o CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/scalar.cpp

CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/scalar.cpp > CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.i

CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/scalar.cpp -o CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.s

CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o: CMakeFiles/l2_distance_16.dir/flags.make
CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o: /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/vector.cpp
CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o: CMakeFiles/l2_distance_16.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o -MF CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o.d -o CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o -c /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/vector.cpp

CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.i"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/vector.cpp > CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.i

CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.s"
	/home/ma9232/.conda/envs/coyoteEnv/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ma9232/TestCoyote/bfv_backend/coyote_out/l2_distance_16/vector.cpp -o CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.s

# Object files for target l2_distance_16
l2_distance_16_OBJECTS = \
"CMakeFiles/l2_distance_16.dir/main.cpp.o" \
"CMakeFiles/l2_distance_16.dir/util.cpp.o" \
"CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o" \
"CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o"

# External object files for target l2_distance_16
l2_distance_16_EXTERNAL_OBJECTS =

l2_distance_16: CMakeFiles/l2_distance_16.dir/main.cpp.o
l2_distance_16: CMakeFiles/l2_distance_16.dir/util.cpp.o
l2_distance_16: CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/scalar.cpp.o
l2_distance_16: CMakeFiles/l2_distance_16.dir/coyote_out/l2_distance_16/vector.cpp.o
l2_distance_16: CMakeFiles/l2_distance_16.dir/build.make
l2_distance_16: /home/ma9232/.conda/envs/coyoteEnv/lib/libseal-3.7.a
l2_distance_16: CMakeFiles/l2_distance_16.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable l2_distance_16"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/l2_distance_16.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/l2_distance_16.dir/build: l2_distance_16
.PHONY : CMakeFiles/l2_distance_16.dir/build

CMakeFiles/l2_distance_16.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/l2_distance_16.dir/cmake_clean.cmake
.PHONY : CMakeFiles/l2_distance_16.dir/clean

CMakeFiles/l2_distance_16.dir/depend:
	cd /home/ma9232/TestCoyote/bfv_backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ma9232/TestCoyote/bfv_backend /home/ma9232/TestCoyote/bfv_backend /home/ma9232/TestCoyote/bfv_backend/build /home/ma9232/TestCoyote/bfv_backend/build /home/ma9232/TestCoyote/bfv_backend/build/CMakeFiles/l2_distance_16.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/l2_distance_16.dir/depend

