#version 330 core

in vec3 outColor; // 从顶点着色器传来的变量（类型和名称要相同）
in vec3 outPosition; // 从顶点着色器传来的顶点坐标
in vec2 outTexture;  // 从顶点着色器传来的纹理坐标

out vec4 color; // 片段着色器输出的变量名可以为任意，但是类型必须为vec4

uniform vec4 ourColor; // uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，全局的
uniform sampler2D ourTexture1; // 通过采样器来获取纹理对象(即贴图)
uniform sampler2D ourTexture2; // 再定义一个采样器来获取另一个纹理

void main()
{
    // color = vec4(outColor, 1.0);
    // color = ourColor;
    // color = vec4(outPosition, 1.0);
    
    // color = texture(ourTexture1, outTexture) * vec4(outColor, 1.0);
    color = mix(texture(ourTexture1, outTexture), texture(ourTexture2, outTexture), 0.5) * vec4(outColor, 1.0); // 0.3表示返回第一个颜色70%和第二个颜色30%的混合色
}

