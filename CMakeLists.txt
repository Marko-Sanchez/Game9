cmake_minimum_required(VERSION 3.10)

project(Game9 CXX)

# Set CPP Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source Files
file(GLOB_RECURSE SOURCE ${CMAKE_SOURCE_DIR}/src/*/*.cpp)

# LSP servers look for this file for linting.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(game9 src/main.cpp ${SOURCE})
target_include_directories(game9 PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(game9
    glfw
    GLEW
    GL
    X11
    GLU
)
