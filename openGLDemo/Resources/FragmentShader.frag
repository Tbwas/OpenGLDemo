#version 330 core

in vec3 vertexColor; // 从顶点着色器传来的变量（类型和名称要相同）
in vec3 outPosition;

out vec4 color; // 片段着色器输出的变量名可以为任意，但是类型必须为vec4

uniform vec4 ourColor; // uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，全局的

void main()
{
    color = vec4(vertexColor, 1.0);
    // color = ourColor;
    // color = vec4(outPosition, 1.0);
}
