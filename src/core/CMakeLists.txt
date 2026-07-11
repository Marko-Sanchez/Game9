add_library(gamenine-core
        world.h
        compositecomponent.h
        window.h
        window.cpp
)

target_include_directories(gamenine-core
        PUBLIC
            ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-core
    PUBLIC
        glm::glm
        glfw
        gamenine-events
)
