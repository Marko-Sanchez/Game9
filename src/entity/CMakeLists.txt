add_library(gamenine-entity
    PlayerBoat.h
    PlayerBoat.cpp
)

target_include_directories(gamenine-entity
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-entity
    PUBLIC
        gamenine-renderer
        gamenine-core
        glm
)
