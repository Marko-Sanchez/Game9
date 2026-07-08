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

# Make sure submodules are present.
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendors/stb/stb_image.h")
    message(FATAL_ERROR
        "Stb submodule missing.\n"
        "Run: git submodule update --init --recursive"
    )
endif()

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG 55f93686c01528224f448c19128836e7df245f72
    FIND_PACKAGE_ARGS 3.12.0
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
    FIND_PACKAGE_ARGS 1.0.1
)

FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(glm)

add_subdirectory(${PROJECT_SOURCE_DIR}/cmake/stb.cmake ${CMAKE_BINARY_DIR}/stb)

add_subdirectory(src)
