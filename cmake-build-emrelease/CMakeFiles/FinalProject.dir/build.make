# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = "/home/brett/Documents/Brock/CS 3P98/Final Project"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease"

# Include any dependencies generated for this target.
include CMakeFiles/FinalProject.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/FinalProject.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/FinalProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FinalProject.dir/flags.make

CMakeFiles/FinalProject.dir/src/main.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/main.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/main.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/main.cpp
CMakeFiles/FinalProject.dir/src/main.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FinalProject.dir/src/main.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/main.cpp.o -MF CMakeFiles/FinalProject.dir/src/main.cpp.o.d -o CMakeFiles/FinalProject.dir/src/main.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/main.cpp"

CMakeFiles/FinalProject.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/main.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/main.cpp" > CMakeFiles/FinalProject.dir/src/main.cpp.i

CMakeFiles/FinalProject.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/main.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/main.cpp" -o CMakeFiles/FinalProject.dir/src/main.cpp.s

CMakeFiles/FinalProject.dir/src/render/camera.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/render/camera.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/render/camera.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/render/camera.cpp
CMakeFiles/FinalProject.dir/src/render/camera.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/FinalProject.dir/src/render/camera.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/render/camera.cpp.o -MF CMakeFiles/FinalProject.dir/src/render/camera.cpp.o.d -o CMakeFiles/FinalProject.dir/src/render/camera.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/camera.cpp"

CMakeFiles/FinalProject.dir/src/render/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/render/camera.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/camera.cpp" > CMakeFiles/FinalProject.dir/src/render/camera.cpp.i

CMakeFiles/FinalProject.dir/src/render/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/render/camera.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/camera.cpp" -o CMakeFiles/FinalProject.dir/src/render/camera.cpp.s

CMakeFiles/FinalProject.dir/src/render/gl.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/render/gl.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/render/gl.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/render/gl.cpp
CMakeFiles/FinalProject.dir/src/render/gl.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/FinalProject.dir/src/render/gl.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/render/gl.cpp.o -MF CMakeFiles/FinalProject.dir/src/render/gl.cpp.o.d -o CMakeFiles/FinalProject.dir/src/render/gl.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/gl.cpp"

CMakeFiles/FinalProject.dir/src/render/gl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/render/gl.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/gl.cpp" > CMakeFiles/FinalProject.dir/src/render/gl.cpp.i

CMakeFiles/FinalProject.dir/src/render/gl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/render/gl.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/gl.cpp" -o CMakeFiles/FinalProject.dir/src/render/gl.cpp.s

CMakeFiles/FinalProject.dir/src/render/textures.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/render/textures.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/render/textures.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/render/textures.cpp
CMakeFiles/FinalProject.dir/src/render/textures.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/FinalProject.dir/src/render/textures.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/render/textures.cpp.o -MF CMakeFiles/FinalProject.dir/src/render/textures.cpp.o.d -o CMakeFiles/FinalProject.dir/src/render/textures.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/textures.cpp"

CMakeFiles/FinalProject.dir/src/render/textures.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/render/textures.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/textures.cpp" > CMakeFiles/FinalProject.dir/src/render/textures.cpp.i

CMakeFiles/FinalProject.dir/src/render/textures.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/render/textures.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/textures.cpp" -o CMakeFiles/FinalProject.dir/src/render/textures.cpp.s

CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/render/ui/debug.cpp
CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o -MF CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o.d -o CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/ui/debug.cpp"

CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/ui/debug.cpp" > CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.i

CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/ui/debug.cpp" -o CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.s

CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/render/ui/text.cpp
CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o -MF CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o.d -o CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/ui/text.cpp"

CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/ui/text.cpp" > CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.i

CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/ui/text.cpp" -o CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.s

CMakeFiles/FinalProject.dir/src/render/window.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/render/window.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/render/window.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/render/window.cpp
CMakeFiles/FinalProject.dir/src/render/window.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/FinalProject.dir/src/render/window.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/render/window.cpp.o -MF CMakeFiles/FinalProject.dir/src/render/window.cpp.o.d -o CMakeFiles/FinalProject.dir/src/render/window.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/window.cpp"

CMakeFiles/FinalProject.dir/src/render/window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/render/window.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/window.cpp" > CMakeFiles/FinalProject.dir/src/render/window.cpp.i

CMakeFiles/FinalProject.dir/src/render/window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/render/window.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/render/window.cpp" -o CMakeFiles/FinalProject.dir/src/render/window.cpp.s

CMakeFiles/FinalProject.dir/src/util/math.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/util/math.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/util/math.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/util/math.cpp
CMakeFiles/FinalProject.dir/src/util/math.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/FinalProject.dir/src/util/math.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/util/math.cpp.o -MF CMakeFiles/FinalProject.dir/src/util/math.cpp.o.d -o CMakeFiles/FinalProject.dir/src/util/math.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/util/math.cpp"

CMakeFiles/FinalProject.dir/src/util/math.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/util/math.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/util/math.cpp" > CMakeFiles/FinalProject.dir/src/util/math.cpp.i

CMakeFiles/FinalProject.dir/src/util/math.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/util/math.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/util/math.cpp" -o CMakeFiles/FinalProject.dir/src/util/math.cpp.s

CMakeFiles/FinalProject.dir/src/util/settings.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/util/settings.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/util/settings.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/util/settings.cpp
CMakeFiles/FinalProject.dir/src/util/settings.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/FinalProject.dir/src/util/settings.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/util/settings.cpp.o -MF CMakeFiles/FinalProject.dir/src/util/settings.cpp.o.d -o CMakeFiles/FinalProject.dir/src/util/settings.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/util/settings.cpp"

CMakeFiles/FinalProject.dir/src/util/settings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/util/settings.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/util/settings.cpp" > CMakeFiles/FinalProject.dir/src/util/settings.cpp.i

CMakeFiles/FinalProject.dir/src/util/settings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/util/settings.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/util/settings.cpp" -o CMakeFiles/FinalProject.dir/src/util/settings.cpp.s

CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/world/chunk/storage.cpp
CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o -MF CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o.d -o CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/chunk/storage.cpp"

CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/chunk/storage.cpp" > CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.i

CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/chunk/storage.cpp" -o CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.s

CMakeFiles/FinalProject.dir/src/world/registry.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/world/registry.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/world/registry.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/world/registry.cpp
CMakeFiles/FinalProject.dir/src/world/registry.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/FinalProject.dir/src/world/registry.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/world/registry.cpp.o -MF CMakeFiles/FinalProject.dir/src/world/registry.cpp.o.d -o CMakeFiles/FinalProject.dir/src/world/registry.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/registry.cpp"

CMakeFiles/FinalProject.dir/src/world/registry.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/world/registry.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/registry.cpp" > CMakeFiles/FinalProject.dir/src/world/registry.cpp.i

CMakeFiles/FinalProject.dir/src/world/registry.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/world/registry.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/registry.cpp" -o CMakeFiles/FinalProject.dir/src/world/registry.cpp.s

CMakeFiles/FinalProject.dir/src/world/world.cpp.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/src/world/world.cpp.o: CMakeFiles/FinalProject.dir/includes_CXX.rsp
CMakeFiles/FinalProject.dir/src/world/world.cpp.o: /home/brett/Documents/Brock/CS\ 3P98/Final\ Project/src/world/world.cpp
CMakeFiles/FinalProject.dir/src/world/world.cpp.o: CMakeFiles/FinalProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/FinalProject.dir/src/world/world.cpp.o"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FinalProject.dir/src/world/world.cpp.o -MF CMakeFiles/FinalProject.dir/src/world/world.cpp.o.d -o CMakeFiles/FinalProject.dir/src/world/world.cpp.o -c "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/world.cpp"

CMakeFiles/FinalProject.dir/src/world/world.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FinalProject.dir/src/world/world.cpp.i"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/world.cpp" > CMakeFiles/FinalProject.dir/src/world/world.cpp.i

CMakeFiles/FinalProject.dir/src/world/world.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FinalProject.dir/src/world/world.cpp.s"
	/usr/bin/em++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/brett/Documents/Brock/CS 3P98/Final Project/src/world/world.cpp" -o CMakeFiles/FinalProject.dir/src/world/world.cpp.s

# Object files for target FinalProject
FinalProject_OBJECTS = \
"CMakeFiles/FinalProject.dir/src/main.cpp.o" \
"CMakeFiles/FinalProject.dir/src/render/camera.cpp.o" \
"CMakeFiles/FinalProject.dir/src/render/gl.cpp.o" \
"CMakeFiles/FinalProject.dir/src/render/textures.cpp.o" \
"CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o" \
"CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o" \
"CMakeFiles/FinalProject.dir/src/render/window.cpp.o" \
"CMakeFiles/FinalProject.dir/src/util/math.cpp.o" \
"CMakeFiles/FinalProject.dir/src/util/settings.cpp.o" \
"CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o" \
"CMakeFiles/FinalProject.dir/src/world/registry.cpp.o" \
"CMakeFiles/FinalProject.dir/src/world/world.cpp.o"

# External object files for target FinalProject
FinalProject_EXTERNAL_OBJECTS =

FinalProject.js: CMakeFiles/FinalProject.dir/src/main.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/render/camera.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/render/gl.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/render/textures.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/render/ui/debug.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/render/ui/text.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/render/window.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/util/math.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/util/settings.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/world/chunk/storage.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/world/registry.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/src/world/world.cpp.o
FinalProject.js: CMakeFiles/FinalProject.dir/build.make
FinalProject.js: libraries/BLT/libBLT.a
FinalProject.js: libraries/freetype-2.13.0/libfreetype.a
FinalProject.js: CMakeFiles/FinalProject.dir/linkLibs.rsp
FinalProject.js: CMakeFiles/FinalProject.dir/objects1
FinalProject.js: CMakeFiles/FinalProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable FinalProject.js"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FinalProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FinalProject.dir/build: FinalProject.js
.PHONY : CMakeFiles/FinalProject.dir/build

CMakeFiles/FinalProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FinalProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FinalProject.dir/clean

CMakeFiles/FinalProject.dir/depend:
	cd "/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/brett/Documents/Brock/CS 3P98/Final Project" "/home/brett/Documents/Brock/CS 3P98/Final Project" "/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease" "/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease" "/home/brett/Documents/Brock/CS 3P98/Final Project/cmake-build-emrelease/CMakeFiles/FinalProject.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/FinalProject.dir/depend

