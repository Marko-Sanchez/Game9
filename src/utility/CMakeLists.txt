add_library(gamenine-utility
    JsonFileHandler.h
    JsonFileHandler.cpp
    Transform.h
    Transform.cpp
)

target_include_directories(gamenine-utility
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-utility
    PUBLIC
        nlohmann_json::nlohmann_json
        glm
)
