# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = "/Applications/CLion 2.app/Contents/bin/cmake/bin/cmake"

# The command to remove a file.
RM = "/Applications/CLion 2.app/Contents/bin/cmake/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/danaklamut/ClionProjects/iondb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/danaklamut/ClionProjects/iondb/cmake-build-debug

# Include any dependencies generated for this target.
include src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/depend.make

# Include the progress variables for this target.
include src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/progress.make

# Include the compile flags for this target's objects.
include src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/flags.make

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/flags.make
src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o: ../src/tests/behaviour/dictionary/behaviour_dictionary.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/danaklamut/ClionProjects/iondb/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o"
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o   -c /Users/danaklamut/ClionProjects/iondb/src/tests/behaviour/dictionary/behaviour_dictionary.c

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.i"
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/danaklamut/ClionProjects/iondb/src/tests/behaviour/dictionary/behaviour_dictionary.c > CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.i

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.s"
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/danaklamut/ClionProjects/iondb/src/tests/behaviour/dictionary/behaviour_dictionary.c -o CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.s

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.requires:

.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.requires

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.provides: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.requires
	$(MAKE) -f src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/build.make src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.provides.build
.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.provides

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.provides.build: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o


src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/flags.make
src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o: ../src/dictionary/ion_master_table.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/danaklamut/ClionProjects/iondb/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o"
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o   -c /Users/danaklamut/ClionProjects/iondb/src/dictionary/ion_master_table.c

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.i"
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/danaklamut/ClionProjects/iondb/src/dictionary/ion_master_table.c > CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.i

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.s"
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && /Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/danaklamut/ClionProjects/iondb/src/dictionary/ion_master_table.c -o CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.s

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.requires:

.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.requires

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.provides: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.requires
	$(MAKE) -f src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/build.make src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.provides.build
.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.provides

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.provides.build: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o


# Object files for target behaviour_dictionary
behaviour_dictionary_OBJECTS = \
"CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o" \
"CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o"

# External object files for target behaviour_dictionary
behaviour_dictionary_EXTERNAL_OBJECTS =

src/tests/behaviour/dictionary/libbehaviour_dictionary.a: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o
src/tests/behaviour/dictionary/libbehaviour_dictionary.a: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o
src/tests/behaviour/dictionary/libbehaviour_dictionary.a: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/build.make
src/tests/behaviour/dictionary/libbehaviour_dictionary.a: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/danaklamut/ClionProjects/iondb/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libbehaviour_dictionary.a"
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && $(CMAKE_COMMAND) -P CMakeFiles/behaviour_dictionary.dir/cmake_clean_target.cmake
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/behaviour_dictionary.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/build: src/tests/behaviour/dictionary/libbehaviour_dictionary.a

.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/build

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/requires: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/behaviour_dictionary.c.o.requires
src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/requires: src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/__/__/__/dictionary/ion_master_table.c.o.requires

.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/requires

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/clean:
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary && $(CMAKE_COMMAND) -P CMakeFiles/behaviour_dictionary.dir/cmake_clean.cmake
.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/clean

src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/depend:
	cd /Users/danaklamut/ClionProjects/iondb/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danaklamut/ClionProjects/iondb /Users/danaklamut/ClionProjects/iondb/src/tests/behaviour/dictionary /Users/danaklamut/ClionProjects/iondb/cmake-build-debug /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary /Users/danaklamut/ClionProjects/iondb/cmake-build-debug/src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tests/behaviour/dictionary/CMakeFiles/behaviour_dictionary.dir/depend

