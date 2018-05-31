#version 330 core

layout(location = 0) out vec4 outColor;

in vec2 TexCoords;

uniform sampler2D textureSampler;
uniform int screenWidth, screenHeight;

vec3 blur();

void main()
{
    vec3 color = texture(textureSampler, TexCoords).rgb;
    outColor = vec4(blur(), 1.0); // 这里并不是直接渲染纹理，而是读取纹理的颜色值
}

vec3 blur() {
    // 根据纹理大小来计算权重
    float dx = 1.0f / screenWidth;
    float dy = 1.0f / screenHeight;
    
    vec3 sum = vec3(0.0f, 0.0f, 0.0f);
    
    // 取相邻值并累加
    for (int i = -3; i < 3; i++) {
        for (int j = -3; j < 3; j++) {
            sum += texture(textureSampler, TexCoords + vec2(i * dx, j * dy)).rgb;
        }
    }
    
    return sum / 30; // 30这个值暂时不知如何取得
}
