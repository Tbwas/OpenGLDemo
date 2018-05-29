#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureSampler;

void main()
{
    vec3 color = texture(textureSampler, TexCoords).rgb;
    FragColor = vec4(color, 1.0); // 这里并不是直接渲染纹理，而是读取纹理的颜色值
//    FragColor = vec4(0.8, 1.0, 0.3, 1.0f);
}
