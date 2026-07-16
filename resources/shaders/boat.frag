#version 330 core

out vec4 f_color;

in vec2 v_TexCoords;
uniform sampler2D u_Texture;

void main()
{
    f_color = texture(u_Texture, v_TexCoords);
}
