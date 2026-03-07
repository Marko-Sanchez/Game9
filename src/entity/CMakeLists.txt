add_library(gamenine-entity
    Train.h
    Train.cpp
)

target_include_directories(gamenine-entity
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-entity
    PUBLIC
        gamenine-renderer
        glm
)
