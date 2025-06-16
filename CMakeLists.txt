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

# Use header files from glm, instead of building (hence we use add_library instead of add_subdirectory).
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/vendors/glm)

target_include_directories(game9 PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/vendors)

target_link_libraries(game9
    glfw
    GLEW
    GL
    X11
    GLU
    glm
)
