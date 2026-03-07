add_subdirectory(events)
add_subdirectory(utility)
add_subdirectory(entity)
add_subdirectory(renderer)
add_subdirectory(layers)
add_subdirectory(managers)
add_subdirectory(core)

add_executable(gamenine main.cpp)

target_include_directories(gamenine
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine
    PRIVATE
        gamenine-layers
)
