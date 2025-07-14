# CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(coroutine_example VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(coroutine_example main.cpp Task.cpp Context.cpp)

target_include_directories(coroutine_example PRIVATE ${PROJECT_SOURCE_DIR})

# Link any libraries if necessary
# target_link_libraries(MyProject some_library)
