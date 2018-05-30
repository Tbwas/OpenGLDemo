#version 330 core

out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D renderTexture;

void main()
{
    outColor = texture(renderTexture, TexCoords);
}
