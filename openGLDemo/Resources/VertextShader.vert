#version 330 core

/*location = 0 在之后的解析顶点数据时会用到，每个顶点属性从一个VBO管理的内存中获得它的数据*/
/*有几个属性，就需要调用`glVertexAttribPointer`函数配置几遍，即该如何解析属性*/
layout(location = 0) in vec3 position; // 位置变量作为顶点属性, 该属性位置为0
layout(location = 1) in vec3 color;    // 颜色变量作为顶点属性, 该属性位置为1
layout(location = 2) in vec2 vTexture;  // 纹理变量作为顶点属性，该属性位置为2

/*当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起*/
out vec3 outColor;  // 将颜色传输给片段着色器
out vec3 outPosition;  // 将顶点坐边传输给片段着色器
out vec2 outTexture;   // 将纹理坐标传输给片段着色器

// uniform float xOffset;

void main() {
    // gl_Position设置的值会成为顶点着色器的输出
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    outColor = color;
    outPosition = position;
    outTexture = vTexture;
}

