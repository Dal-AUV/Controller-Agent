# Build Toolchain configuration
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(Controller-Agent)

# Specify the source files and include directories
set(SOURCE_FILES src/main.cpp src/serial_if.cpp)
include_directories(inc)

find_package(Boost REQUIRED COMPONENTS system thread)

# Create an executable target named "hello_world" from the source files
add_executable(Controller-Agent ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(Controller-Agent PRIVATE ${Boost_LIBRARIES})
target_include_directories(Controller-Agent PRIVATE inc)

# Specify the output dir for the exe
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)