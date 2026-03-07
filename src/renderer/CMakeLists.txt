add_library(gamenine-renderer
    Shader.h
    Shader.cpp
    SpriteRenderer.h
    SpriteRenderer.cpp
    Texture2D.h
    Texture2D.cpp
    stb_image.cpp
)

target_include_directories(gamenine-renderer
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(gamenine-renderer
    PUBLIC
        stb
        gamenine-utility
        GLEW
        GL
        glm
)
