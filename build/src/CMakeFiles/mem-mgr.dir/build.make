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
include src/CMakeFiles/mem-mgr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/mem-mgr.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/mem-mgr.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/mem-mgr.dir/flags.make

src/CMakeFiles/mem-mgr.dir/codegen:
.PHONY : src/CMakeFiles/mem-mgr.dir/codegen

src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o: src/CMakeFiles/mem-mgr.dir/flags.make
src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o: /home/thedegeo/Documents/T/MPointers2/src/memory_manager/main.cpp
src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o: src/CMakeFiles/mem-mgr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/thedegeo/Documents/T/MPointers2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o"
	cd /home/thedegeo/Documents/T/MPointers2/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o -MF CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o.d -o CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o -c /home/thedegeo/Documents/T/MPointers2/src/memory_manager/main.cpp

src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.i"
	cd /home/thedegeo/Documents/T/MPointers2/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/thedegeo/Documents/T/MPointers2/src/memory_manager/main.cpp > CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.i

src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.s"
	cd /home/thedegeo/Documents/T/MPointers2/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/thedegeo/Documents/T/MPointers2/src/memory_manager/main.cpp -o CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.s

# Object files for target mem-mgr
mem__mgr_OBJECTS = \
"CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o"

# External object files for target mem-mgr
mem__mgr_EXTERNAL_OBJECTS =

src/mem-mgr: src/CMakeFiles/mem-mgr.dir/memory_manager/main.cpp.o
src/mem-mgr: src/CMakeFiles/mem-mgr.dir/build.make
src/mem-mgr: src/CMakeFiles/mem-mgr.dir/compiler_depend.ts
src/mem-mgr: src/memory_manager/libmemory_manager.a
src/mem-mgr: src/mpointers/libmpointers.a
src/mem-mgr: src/proto/libproto_lib.a
src/mem-mgr: /usr/local/lib/libgrpc++_unsecure.a
src/mem-mgr: /usr/local/lib/libgrpc_unsecure.a
src/mem-mgr: /usr/local/lib/libgpr.a
src/mem-mgr: src/proto/libproto_lib.a
src/mem-mgr: src/mpointers/libmpointers.a
src/mem-mgr: src/memory_manager/libmemory_manager.a
src/mem-mgr: /usr/local/lib/libgrpc++.a
src/mem-mgr: /usr/local/lib/libgrpc++_unsecure.a
src/mem-mgr: /usr/local/lib/libgrpc.a
src/mem-mgr: /usr/local/lib/libgrpc_unsecure.a
src/mem-mgr: src/proto/libproto_lib.a
src/mem-mgr: /usr/local/lib/libgrpc++.a
src/mem-mgr: /usr/local/lib/libgrpc.a
src/mem-mgr: /usr/local/lib/libupb_json_lib.a
src/mem-mgr: /usr/local/lib/libupb_textformat_lib.a
src/mem-mgr: /usr/local/lib/libre2.a
src/mem-mgr: /usr/local/lib/libssl.a
src/mem-mgr: /usr/local/lib/libcrypto.a
src/mem-mgr: /usr/lib/libprotobuf.so.25.1.0
src/mem-mgr: /usr/lib/libabsl_log_internal_check_op.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_leak_check.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_die_if_null.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_internal_conditions.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_internal_message.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_internal_nullguard.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_examine_stack.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_internal_format.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_internal_proto.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_internal_log_sink_set.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_sink.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_entry.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_initialize.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_globals.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_internal_globals.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_statusor.so.2308.0.0
src/mem-mgr: /usr/local/lib/libupb_collections_lib.a
src/mem-mgr: /usr/local/lib/libupb.a
src/mem-mgr: /usr/local/lib/libutf8_range_lib.a
src/mem-mgr: /usr/local/lib/libz.a
src/mem-mgr: /usr/local/lib/libcares.a
src/mem-mgr: /usr/local/lib/libaddress_sorting.a
src/mem-mgr: /usr/local/lib/libgpr.a
src/mem-mgr: /usr/lib/libabsl_flags.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_reflection.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_raw_hash_set.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_hash.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_city.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_low_level_hash.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_hashtablez_sampler.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_config.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_program_name.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_private_handle_accessor.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_commandlineflag.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_commandlineflag_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_status.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_cord.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_cordz_info.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_cord_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_cordz_functions.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_exponential_biased.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_cordz_handle.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_crc_cord_state.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_crc32c.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_crc_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_crc_cpu_detect.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_strerror.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_synchronization.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_stacktrace.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_symbolize.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_debugging_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_demangle_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_graphcycles_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_kernel_timeout_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_malloc_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_time.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_civil_time.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_time_zone.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_bad_variant_access.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_flags_marshalling.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_distributions.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_seed_sequences.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_internal_pool_urbg.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_internal_randen.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_internal_randen_hwaes.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_internal_randen_hwaes_impl.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_internal_randen_slow.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_internal_platform.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_internal_seed_material.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_bad_optional_access.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_random_seed_gen_exception.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_str_format_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_strings.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_string_view.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_strings_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_base.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_spinlock_wait.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_throw_delegate.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_raw_logging_internal.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_log_severity.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_int128.so.2308.0.0
src/mem-mgr: /usr/lib/libabsl_statusor.so
src/mem-mgr: /usr/lib/libabsl_status.so
src/mem-mgr: /usr/lib/libabsl_log_internal_check_op.so
src/mem-mgr: /usr/lib/libabsl_log_internal_message.so
src/mem-mgr: /usr/lib/libabsl_examine_stack.so
src/mem-mgr: /usr/lib/libabsl_strerror.so
src/mem-mgr: /usr/lib/libabsl_log_internal_conditions.so
src/mem-mgr: /usr/lib/libabsl_log_internal_format.so
src/mem-mgr: /usr/lib/libabsl_log_internal_log_sink_set.so
src/mem-mgr: /usr/lib/libabsl_log_internal_globals.so
src/mem-mgr: /usr/lib/libabsl_log_globals.so
src/mem-mgr: /usr/lib/libabsl_log_sink.so
src/mem-mgr: /usr/lib/libabsl_log_entry.so
src/mem-mgr: /usr/lib/libabsl_log_internal_nullguard.so
src/mem-mgr: /usr/lib/libabsl_log_internal_proto.so
src/mem-mgr: /usr/lib/libre2.so
src/mem-mgr: /usr/lib/libabsl_flags.so
src/mem-mgr: /usr/lib/libabsl_flags_internal.so
src/mem-mgr: /usr/lib/libabsl_flags_marshalling.so
src/mem-mgr: /usr/lib/libabsl_flags_reflection.so
src/mem-mgr: /usr/lib/libabsl_flags_private_handle_accessor.so
src/mem-mgr: /usr/lib/libabsl_flags_commandlineflag.so
src/mem-mgr: /usr/lib/libabsl_flags_commandlineflag_internal.so
src/mem-mgr: /usr/lib/libabsl_flags_config.so
src/mem-mgr: /usr/lib/libabsl_flags_program_name.so
src/mem-mgr: /usr/lib/libabsl_cord.so
src/mem-mgr: /usr/lib/libabsl_cordz_info.so
src/mem-mgr: /usr/lib/libabsl_cord_internal.so
src/mem-mgr: /usr/lib/libabsl_cordz_functions.so
src/mem-mgr: /usr/lib/libabsl_cordz_handle.so
src/mem-mgr: /usr/lib/libabsl_crc_cord_state.so
src/mem-mgr: /usr/lib/libabsl_crc32c.so
src/mem-mgr: /usr/lib/libabsl_crc_internal.so
src/mem-mgr: /usr/lib/libabsl_crc_cpu_detect.so
src/mem-mgr: /usr/lib/libabsl_raw_hash_set.so
src/mem-mgr: /usr/lib/libabsl_hash.so
src/mem-mgr: /usr/lib/libabsl_city.so
src/mem-mgr: /usr/lib/libabsl_bad_variant_access.so
src/mem-mgr: /usr/lib/libabsl_low_level_hash.so
src/mem-mgr: /usr/lib/libabsl_hashtablez_sampler.so
src/mem-mgr: /usr/lib/libabsl_exponential_biased.so
src/mem-mgr: /usr/lib/libabsl_str_format_internal.so
src/mem-mgr: /usr/lib/libabsl_bad_optional_access.so
src/mem-mgr: /usr/lib/libabsl_synchronization.so
src/mem-mgr: /usr/lib/libabsl_graphcycles_internal.so
src/mem-mgr: /usr/lib/libabsl_kernel_timeout_internal.so
src/mem-mgr: /usr/lib/libabsl_stacktrace.so
src/mem-mgr: /usr/lib/libabsl_symbolize.so
src/mem-mgr: /usr/lib/libabsl_debugging_internal.so
src/mem-mgr: /usr/lib/libabsl_demangle_internal.so
src/mem-mgr: /usr/lib/libabsl_malloc_internal.so
src/mem-mgr: /usr/lib/libabsl_time.so
src/mem-mgr: /usr/lib/libabsl_civil_time.so
src/mem-mgr: /usr/lib/libabsl_time_zone.so
src/mem-mgr: /usr/lib/libabsl_strings.so
src/mem-mgr: /usr/lib/libabsl_string_view.so
src/mem-mgr: /usr/lib/libabsl_strings_internal.so
src/mem-mgr: /usr/lib/libabsl_base.so
src/mem-mgr: /usr/lib/librt.a
src/mem-mgr: /usr/lib/libabsl_spinlock_wait.so
src/mem-mgr: /usr/lib/libabsl_int128.so
src/mem-mgr: /usr/lib/libabsl_throw_delegate.so
src/mem-mgr: /usr/lib/libabsl_raw_logging_internal.so
src/mem-mgr: /usr/lib/libabsl_log_severity.so
src/mem-mgr: src/CMakeFiles/mem-mgr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/thedegeo/Documents/T/MPointers2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mem-mgr"
	cd /home/thedegeo/Documents/T/MPointers2/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mem-mgr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/mem-mgr.dir/build: src/mem-mgr
.PHONY : src/CMakeFiles/mem-mgr.dir/build

src/CMakeFiles/mem-mgr.dir/clean:
	cd /home/thedegeo/Documents/T/MPointers2/build/src && $(CMAKE_COMMAND) -P CMakeFiles/mem-mgr.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/mem-mgr.dir/clean

src/CMakeFiles/mem-mgr.dir/depend:
	cd /home/thedegeo/Documents/T/MPointers2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thedegeo/Documents/T/MPointers2 /home/thedegeo/Documents/T/MPointers2/src /home/thedegeo/Documents/T/MPointers2/build /home/thedegeo/Documents/T/MPointers2/build/src /home/thedegeo/Documents/T/MPointers2/build/src/CMakeFiles/mem-mgr.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/mem-mgr.dir/depend

