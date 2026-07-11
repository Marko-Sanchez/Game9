#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexcoords;

layout (location = 2) in vec2 aInstanceTranslation;

out vec2 v_Texcoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_Texcoords = aTexcoords;
    vec4 modelPos = u_Model * vec4(aPosition, 0.0f, 1.0f);
    vec4 worldPos = modelPos + vec4(aInstanceTranslation, 0.0f, 0.0f);
    gl_Position = u_Projection * u_View * worldPos;
}
