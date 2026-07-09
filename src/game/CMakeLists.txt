add_library(gamenine-game
    Game.h
    Game.cpp
)

target_include_directories(gamenine-game
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-game
    PUBLIC
        GL
        gamenine-core
)
