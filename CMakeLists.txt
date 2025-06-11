cmake_minimum_required(VERSION 3.10)

project(Game9 CXX)

# Set CPP Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE src/main.cpp)

# LSP servers look for this file for linting.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(game9 ${SOURCE})
