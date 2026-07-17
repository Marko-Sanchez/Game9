add_library(gamenine-scene
    OceanMap.h
    OceanMap.cpp
)

target_include_directories(gamenine-scene
        PUBLIC
            ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-scene
    PUBLIC
        gamenine-entity
        gamenine-core
        gamenine-renderer
)
