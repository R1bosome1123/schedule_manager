# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/pocket/my_cpp_program/HW04/Backend

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pocket/my_cpp_program/HW04/Backend/build

# Include any dependencies generated for this target.
include CMakeFiles/Backend.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Backend.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Backend.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Backend.dir/flags.make

CMakeFiles/Backend.dir/src/info_prompt.cpp.o: CMakeFiles/Backend.dir/flags.make
CMakeFiles/Backend.dir/src/info_prompt.cpp.o: ../src/info_prompt.cpp
CMakeFiles/Backend.dir/src/info_prompt.cpp.o: CMakeFiles/Backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pocket/my_cpp_program/HW04/Backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Backend.dir/src/info_prompt.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Backend.dir/src/info_prompt.cpp.o -MF CMakeFiles/Backend.dir/src/info_prompt.cpp.o.d -o CMakeFiles/Backend.dir/src/info_prompt.cpp.o -c /home/pocket/my_cpp_program/HW04/Backend/src/info_prompt.cpp

CMakeFiles/Backend.dir/src/info_prompt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Backend.dir/src/info_prompt.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pocket/my_cpp_program/HW04/Backend/src/info_prompt.cpp > CMakeFiles/Backend.dir/src/info_prompt.cpp.i

CMakeFiles/Backend.dir/src/info_prompt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Backend.dir/src/info_prompt.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pocket/my_cpp_program/HW04/Backend/src/info_prompt.cpp -o CMakeFiles/Backend.dir/src/info_prompt.cpp.s

CMakeFiles/Backend.dir/src/main.cpp.o: CMakeFiles/Backend.dir/flags.make
CMakeFiles/Backend.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/Backend.dir/src/main.cpp.o: CMakeFiles/Backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pocket/my_cpp_program/HW04/Backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Backend.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Backend.dir/src/main.cpp.o -MF CMakeFiles/Backend.dir/src/main.cpp.o.d -o CMakeFiles/Backend.dir/src/main.cpp.o -c /home/pocket/my_cpp_program/HW04/Backend/src/main.cpp

CMakeFiles/Backend.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Backend.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pocket/my_cpp_program/HW04/Backend/src/main.cpp > CMakeFiles/Backend.dir/src/main.cpp.i

CMakeFiles/Backend.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Backend.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pocket/my_cpp_program/HW04/Backend/src/main.cpp -o CMakeFiles/Backend.dir/src/main.cpp.s

CMakeFiles/Backend.dir/src/server.cpp.o: CMakeFiles/Backend.dir/flags.make
CMakeFiles/Backend.dir/src/server.cpp.o: ../src/server.cpp
CMakeFiles/Backend.dir/src/server.cpp.o: CMakeFiles/Backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pocket/my_cpp_program/HW04/Backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Backend.dir/src/server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Backend.dir/src/server.cpp.o -MF CMakeFiles/Backend.dir/src/server.cpp.o.d -o CMakeFiles/Backend.dir/src/server.cpp.o -c /home/pocket/my_cpp_program/HW04/Backend/src/server.cpp

CMakeFiles/Backend.dir/src/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Backend.dir/src/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pocket/my_cpp_program/HW04/Backend/src/server.cpp > CMakeFiles/Backend.dir/src/server.cpp.i

CMakeFiles/Backend.dir/src/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Backend.dir/src/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pocket/my_cpp_program/HW04/Backend/src/server.cpp -o CMakeFiles/Backend.dir/src/server.cpp.s

CMakeFiles/Backend.dir/src/task.cc.o: CMakeFiles/Backend.dir/flags.make
CMakeFiles/Backend.dir/src/task.cc.o: ../src/task.cc
CMakeFiles/Backend.dir/src/task.cc.o: CMakeFiles/Backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pocket/my_cpp_program/HW04/Backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Backend.dir/src/task.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Backend.dir/src/task.cc.o -MF CMakeFiles/Backend.dir/src/task.cc.o.d -o CMakeFiles/Backend.dir/src/task.cc.o -c /home/pocket/my_cpp_program/HW04/Backend/src/task.cc

CMakeFiles/Backend.dir/src/task.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Backend.dir/src/task.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pocket/my_cpp_program/HW04/Backend/src/task.cc > CMakeFiles/Backend.dir/src/task.cc.i

CMakeFiles/Backend.dir/src/task.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Backend.dir/src/task.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pocket/my_cpp_program/HW04/Backend/src/task.cc -o CMakeFiles/Backend.dir/src/task.cc.s

CMakeFiles/Backend.dir/src/useraccount.cc.o: CMakeFiles/Backend.dir/flags.make
CMakeFiles/Backend.dir/src/useraccount.cc.o: ../src/useraccount.cc
CMakeFiles/Backend.dir/src/useraccount.cc.o: CMakeFiles/Backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pocket/my_cpp_program/HW04/Backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Backend.dir/src/useraccount.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Backend.dir/src/useraccount.cc.o -MF CMakeFiles/Backend.dir/src/useraccount.cc.o.d -o CMakeFiles/Backend.dir/src/useraccount.cc.o -c /home/pocket/my_cpp_program/HW04/Backend/src/useraccount.cc

CMakeFiles/Backend.dir/src/useraccount.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Backend.dir/src/useraccount.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pocket/my_cpp_program/HW04/Backend/src/useraccount.cc > CMakeFiles/Backend.dir/src/useraccount.cc.i

CMakeFiles/Backend.dir/src/useraccount.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Backend.dir/src/useraccount.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pocket/my_cpp_program/HW04/Backend/src/useraccount.cc -o CMakeFiles/Backend.dir/src/useraccount.cc.s

# Object files for target Backend
Backend_OBJECTS = \
"CMakeFiles/Backend.dir/src/info_prompt.cpp.o" \
"CMakeFiles/Backend.dir/src/main.cpp.o" \
"CMakeFiles/Backend.dir/src/server.cpp.o" \
"CMakeFiles/Backend.dir/src/task.cc.o" \
"CMakeFiles/Backend.dir/src/useraccount.cc.o"

# External object files for target Backend
Backend_EXTERNAL_OBJECTS =

bin/Backend: CMakeFiles/Backend.dir/src/info_prompt.cpp.o
bin/Backend: CMakeFiles/Backend.dir/src/main.cpp.o
bin/Backend: CMakeFiles/Backend.dir/src/server.cpp.o
bin/Backend: CMakeFiles/Backend.dir/src/task.cc.o
bin/Backend: CMakeFiles/Backend.dir/src/useraccount.cc.o
bin/Backend: CMakeFiles/Backend.dir/build.make
bin/Backend: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.74.0
bin/Backend: /usr/lib/x86_64-linux-gnu/libssl.so
bin/Backend: /usr/lib/x86_64-linux-gnu/libcrypto.so
bin/Backend: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.15.3
bin/Backend: CMakeFiles/Backend.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pocket/my_cpp_program/HW04/Backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable bin/Backend"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Backend.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Backend.dir/build: bin/Backend
.PHONY : CMakeFiles/Backend.dir/build

CMakeFiles/Backend.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Backend.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Backend.dir/clean

CMakeFiles/Backend.dir/depend:
	cd /home/pocket/my_cpp_program/HW04/Backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pocket/my_cpp_program/HW04/Backend /home/pocket/my_cpp_program/HW04/Backend /home/pocket/my_cpp_program/HW04/Backend/build /home/pocket/my_cpp_program/HW04/Backend/build /home/pocket/my_cpp_program/HW04/Backend/build/CMakeFiles/Backend.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Backend.dir/depend

