cmake_minimum_required(VERSION 3.20)

project(VoltaEngine LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.2
)

FetchContent_MakeAvailable(SDL3)

# glad
add_library(glad external/glad/glad.c)
target_include_directories(glad PUBLIC external)

# engine sources
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(app
    ${ENGINE_SOURCES}
    examples/TriangleAnim.cpp
)

target_link_libraries(app
    PRIVATE
    SDL3::SDL3
    glad
)
