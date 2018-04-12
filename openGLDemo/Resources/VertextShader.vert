#version 330 core

/*location = 0 在之后的解析顶点数据时会用到，每个顶点属性从一个VBO管理的内存中获得它的数据*/
/*有几个属性，就需要调用`glVertexAttribPointer`函数配置几遍，即该如何解析属性*/
layout(location = 0) in vec3 position; // 位置变量作为顶点属性, 该属性位置为0
layout(location = 1) in vec3 normalVector; // 法线向量属性，该属性位置为1
layout(location = 2) in vec2 vTexture; // 纹理变量作为顶点属性，该属性位置为2

/*当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起*/
out vec3 outPosition;  // 将顶点坐边传输给片段着色器
out vec2 outTexture;   // 将纹理坐标传输给片段着色器
out vec3 outNormalVec; // 将法向量传递到片段着色器
out vec3 outFragPosition; // 将片段的位置传递到片段着色器

// uniform float xOffset;
uniform mat4 trans; // 矩阵变换
uniform mat4 model; // 模型矩阵
uniform mat4 view;  // 视觉矩阵
uniform mat4 projection; // 投影矩阵

void main() {
    // gl_Position设置的值会成为顶点着色器的输出
    // gl_Position = vec4(position, 1.0);
    gl_Position = projection * trans * vec4(position, 1.0);
    // gl_Position = projection * trans * view * model * vec4(position.x, position.y, position.z, 1.0);
    // gl_Position = trans * vec4(position.x, position.y, position.z, 1.0);
    // gl_Position = projection * view * model * vec4(position, 1.0);
    outPosition = position;
    outTexture = vTexture;
    outNormalVec = normalVector;
    outFragPosition = vec3(trans * vec4(position, 1.0)); // 计算片段位置向量有点难以理解
}
