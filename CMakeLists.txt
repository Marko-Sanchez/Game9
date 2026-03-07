cmake_minimum_required(VERSION 3.25)

project(Game9
    LANGUAGES CXX
    DESCRIPTION "The 9th Game in the series"
    HOMEPAGE_URL "https://github.com/Marko-Sanchez/Game9"
)

include(CTest)

# Set CPP Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Threads needed; Use pthreads if possible,
# finds preferred thread library of the system ensuring project builds correctly across different platforms.
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${PROJECT_SOURCE_DIR}/vendors/stb)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendors/glm)

add_subdirectory(${PROJECT_SOURCE_DIR}/vendors/json
                ${CMAKE_BINARY_DIR}/nlohmann_json)

add_subdirectory(src)
