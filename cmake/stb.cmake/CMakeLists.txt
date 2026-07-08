add_library(stb STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp
)

target_include_directories(stb PUBLIC
    ${PROJECT_SOURCE_DIR}/vendors/stb
)
