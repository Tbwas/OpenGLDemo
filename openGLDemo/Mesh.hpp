//
//  Mesh.hpp
//  openGLDemo
//
//  Created by xindong on 2018/4/25.
//  Copyright © 2018年 xindong. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "glm.hpp"
#include <OpenGL/gl3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TexCoords;
}; // C++结构体有一个很棒的特性，它们的内存布局是连续的(Sequential)。也就是说，如果我们将结构体作为一个数据数组使用，那么它将会以顺序排列结构体的变量.

struct Texture {
    uint id; // 纹理对象的名字，由`glGenTextures()`生成.
    string type;
    aiString path; // 存储加载过的纹理路劲，避免同一个纹理加载多次，进行性能优化
};

/**
 一个Mesh对象本身包含了渲染所需要的所有相关数据，像是顶点位置、法向量、纹理坐标、面(Face)和物体的材质. 一个网格是我们在OpenGL中绘制物体所需的最小单位（顶点数据、索引和材质属性）。一个模型（通常）会包括多个网格
 */
class Mesh {
    
public:
    /* 网格数据 */
    vector<Vertex> vertices; // vector(向量): C++中的一种数据结构,确切的说是一个类.它相当于一个动态的数组
    vector<uint> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures);
    void Draw(GLuint shaderProgram);
    
private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};

#endif
