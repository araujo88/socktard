# Set the minimum version of CMake that's required
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(socktard VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Can manually add the sources using the set command as follows:
# set(SOURCES src/main.cpp src/logger.cpp src/console_logger.cpp src/file_logger.cpp)

# However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

# Add the executable
add_executable(socktard ${SOURCES})

# Include the FetchContent module
include(FetchContent)

# Declare the dependency using FetchContent
FetchContent_Declare(
  logtard
  GIT_REPOSITORY https://github.com/araujo88/logtard.git
  GIT_TAG        main
)

# Make the content available
FetchContent_MakeAvailable(logtard)

# Link the logtard library target to your executable
target_link_libraries(socktard PRIVATE logtard)
