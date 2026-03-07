add_library(gamenine-core
    Game.h
    Game.cpp
    window.h
    window.cpp
)

target_include_directories(gamenine-core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-core
    PUBLIC
        glfw
        GL
        X11
)
