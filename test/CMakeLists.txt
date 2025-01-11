cmake_minimum_required(VERSION 3.10)
project(monolith-tests)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(../monolith)

# Add sources
file(GLOB_RECURSE MONOLITH_TEST_SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE MONOLITH_SHELL_SOURCES CONFIGURE_DEPENDS ../monolith/*.cpp)

# Create test executable
add_executable(${PROJECT_NAME} ${MONOLITH_TEST_SOURCES} ${MONOLITH_SHELL_SOURCES})
