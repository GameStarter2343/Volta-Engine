cmake_minimum_required(VERSION 3.20)
project(TriangleAnimation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "external/glad/glad.c" "examples/TriangleAnim.cpp")

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE external)

find_package(SDL3 REQUIRED)
target_link_libraries(app PRIVATE SDL3::SDL3)
