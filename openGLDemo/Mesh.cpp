//
//  Mesh.cpp
//  openGLDemo
//
//  Created by xindong on 2018/4/25.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "Mesh.hpp"

#define dx_offsetof(t, m) ((size_t) ((char *)&((st *)(0))->m - (char *)0))

#pragma mark - Public

Mesh::Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    
    setupMesh();
}

void Mesh::Draw(GLuint shaderProgram) {
    uint diffuseNr = 1;
    uint specularNr = 1;
    
    for (uint i = 0; i < textures.size(); i++) {
        // 将采样器和纹理单元对应上
        string number; // 获取纹理序号（diffuse_textureN中的N）
        string type = textures[i].type;
        if (type == "texture_diffuse") {
            number = to_string(diffuseNr++);
        } else if (type == "texture_specular") {
            number = to_string(specularNr++);
        }
        string name = "material." + type + number;
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), i);
    }
    
    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#pragma mark - Private

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);
    
    // 顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    
    // 法向量属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // `offsetof` is the same as `dx_offsetof`.
    glEnableVertexAttribArray(1);
    
    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}
