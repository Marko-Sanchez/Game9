cmake_minimum_required(VERSION 3.18)

project(Game9
    DESCRIPTION "The 9th Game in the series"
    LANGUAGES CXX)

# Set CPP Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source Files
file(GLOB_RECURSE SOURCE ${CMAKE_SOURCE_DIR}/src/*/*.cpp)

add_executable(game9 src/main.cpp ${SOURCE})

# Cmake build type for single-config generators.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(game9 PRIVATE -g -O1 -Wall -Wextra -Wpedantic)
else()
    target_compile_options(game9 PRIVATE -O2)
endif()

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

# steps:
# cmake -B build -DCMAKE_INSTALL_PREFIX=$HOME .
# cmake --build build
# cmake --install build

# Install files needed to display custom app icon.
install(FILES "${CMAKE_SOURCE_DIR}/custom.game9.desktop"
        DESTINATION "/usr/share/applications")

install(FILES "${CMAKE_SOURCE_DIR}/resources/images/winIcon64.png"
          RENAME appicon-game9.png
          DESTINATION "/usr/share/icons/hicolor/64x64/apps/")

install(TARGETS game9 RUNTIME DESTINATION ~/bin)# temp: install binary to /home/marku/bin
