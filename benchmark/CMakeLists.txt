cmake_minimum_required(VERSION 3.15)
project(Project)

# Get the absolute path of the benchmark source directory
get_filename_component(MONOLITH_BENCH_SOURCE_PATH "./monolith" ABSOLUTE)

# Base name for the benchmark targets
set(MONOLITH_BENCH_TARGET_BASE "Project-bench")

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0") # For debugging
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

# Macro to declare benchmark targets
macro(MONOLITH_BENCH_DECLARE suffix file)
    add_executable(
        ${MONOLITH_BENCH_TARGET_BASE}-${suffix}
        ${MONOLITH_BENCH_SOURCE_PATH}/${file}
        ${MONOLITH_BENCH_SOURCE_PATH}/Fun.cpp
    )

    target_link_libraries(
        ${MONOLITH_BENCH_TARGET_BASE}-${suffix}
        pthread
    )

    # Add debugging flags for compilation
    target_compile_options(
        ${MONOLITH_BENCH_TARGET_BASE}-${suffix}
        PRIVATE
        -g -O0
    )
endmacro()

# Declare benchmark executables
MONOLITH_BENCH_DECLARE(factorizing FactorizeBench.cpp)
MONOLITH_BENCH_DECLARE(search-name SearchByNameBench.cpp)
MONOLITH_BENCH_DECLARE(multi MultiThreadedBench.cpp)
