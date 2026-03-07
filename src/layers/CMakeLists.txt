add_library(gamenine-layers
    Layer.h
    Layer.cpp
    BackgroundLayer.h
    BackgroundLayer.cpp
    TrainLayer.h
    TrainLayer.cpp
)

target_include_directories(gamenine-layers
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-layers
    PUBLIC
        gamenine-events
        gamenine-managers
        GLEW
        glm
        gamenine-core
)
