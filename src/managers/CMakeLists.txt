add_library(gamenine-managers
    ResourceManager.h
    ResourceManager.cpp
    SceneHandler.h
    SceneHandler.cpp
    TrainHandler.h
    TrainHandler.cpp
)

target_include_directories(gamenine-managers
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-managers
    PUBLIC
        gamenine-renderer
        gamenine-entity
        nlohmann_json::nlohmann_json
)
