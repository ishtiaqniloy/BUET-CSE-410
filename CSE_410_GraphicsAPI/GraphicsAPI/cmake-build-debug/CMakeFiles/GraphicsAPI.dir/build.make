# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\Ishti\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\191.6707.69\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\Ishti\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\191.6707.69\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/GraphicsAPI.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/GraphicsAPI.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GraphicsAPI.dir/flags.make

CMakeFiles/GraphicsAPI.dir/main.cpp.obj: CMakeFiles/GraphicsAPI.dir/flags.make
CMakeFiles/GraphicsAPI.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GraphicsAPI.dir/main.cpp.obj"
	C:\PROGRA~2\CodeBlocks\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\GraphicsAPI.dir\main.cpp.obj -c "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\main.cpp"

CMakeFiles/GraphicsAPI.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GraphicsAPI.dir/main.cpp.i"
	C:\PROGRA~2\CodeBlocks\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\main.cpp" > CMakeFiles\GraphicsAPI.dir\main.cpp.i

CMakeFiles/GraphicsAPI.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GraphicsAPI.dir/main.cpp.s"
	C:\PROGRA~2\CodeBlocks\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\main.cpp" -o CMakeFiles\GraphicsAPI.dir\main.cpp.s

# Object files for target GraphicsAPI
GraphicsAPI_OBJECTS = \
"CMakeFiles/GraphicsAPI.dir/main.cpp.obj"

# External object files for target GraphicsAPI
GraphicsAPI_EXTERNAL_OBJECTS =

GraphicsAPI.exe: CMakeFiles/GraphicsAPI.dir/main.cpp.obj
GraphicsAPI.exe: CMakeFiles/GraphicsAPI.dir/build.make
GraphicsAPI.exe: CMakeFiles/GraphicsAPI.dir/linklibs.rsp
GraphicsAPI.exe: CMakeFiles/GraphicsAPI.dir/objects1.rsp
GraphicsAPI.exe: CMakeFiles/GraphicsAPI.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GraphicsAPI.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\GraphicsAPI.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GraphicsAPI.dir/build: GraphicsAPI.exe

.PHONY : CMakeFiles/GraphicsAPI.dir/build

CMakeFiles/GraphicsAPI.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\GraphicsAPI.dir\cmake_clean.cmake
.PHONY : CMakeFiles/GraphicsAPI.dir/clean

CMakeFiles/GraphicsAPI.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI" "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI" "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\cmake-build-debug" "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\cmake-build-debug" "D:\Drive\L-4 T-1\CSE 410 - Computer Graphics Sessional\Assignment 2\CSE_410_Assignment2_GraphicsAPI\GraphicsAPI\cmake-build-debug\CMakeFiles\GraphicsAPI.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/GraphicsAPI.dir/depend

