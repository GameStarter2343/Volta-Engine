include(FetchContent)

cmake_minimum_required(VERSION 3.20)
project(Volta-Engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "external/glad/glad.c" "examples/TriangleAnim.cpp")

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE external)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.2
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(SDL3)
target_link_libraries(app PRIVATE SDL3::SDL3)
