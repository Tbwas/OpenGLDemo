//
//  Model.cpp
//  openGLDemo
//
//  Created by xindong on 2018/4/25.
//  Copyright © 2018年 xindong. All rights reserved.
//

/****
 @param aiProcess_Triangulate: 如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形
 @param aiProcess_FlipUVs: 将在处理的时候翻转y轴的纹理坐标
 @param aiProcess_GenNormals: 如果模型不包含法向量的话，就为每个顶点创建法线
 @param aiProcess_SplitLargeMeshes: 将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用
 @param aiProcess_OptimizeMeshes: 和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化
 
 More details see as http://assimp.sourceforge.net/lib_html/postprocess_8h.html
**/

#include "Model.hpp"
#include <iostream>
#include "stb_image.h"

// 存储加载过的纹理结构体
vector<Texture> textures_loaded;

uint TextureFromFile(const char *path, const string directory, GLuint textureUnit);

#pragma mark - Public

void Model::Draw(GLuint shaderProgram) {
    if (meshes.size() == 0) {
        cout << "ERROR: no meshes." << endl;
        abort();
    }
    for (int i = 0; i < meshes.size(); i++) {
        Mesh mesh = meshes[i];
        mesh.Draw(shaderProgram);
    }
}

#pragma mark - Private

void Model::loadModel(string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        abort();
    }
    
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene); // 每个节点（可能）包含有多个子节点，我们希望首先处理参数中的节点，再继续处理该节点所有的子节点，以此类推.
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    node->mNumMeshes = 7;
    if (node->mNumMeshes == 0) {
        cout << "ERROR: current node no meshes." << endl;
        abort();
    }
    
    // 处理节点所有的网格
    for (uint i = 0; i < node->mNumMeshes; i++) {
        
        // @Note: node中的mMeshes数组保存的只是scene中网格数组的索引, 而scene下的mMeshes数组储存了真正的Mesh对象.
        aiMesh *mesh = scene->mMeshes[i];
        meshes.push_back(processMesh(mesh, scene));
    }
    
    // 递归子节点
    for (uint i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
    cout << "Node process complete." << endl;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    // 处理顶点数据
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        
        // 位置
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        
        // 法向量
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;
        
        // 纹理坐标
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            cout << "当前mesh中不包含纹理坐标" << endl;
            vertex.TexCoords.x = 0.0f;
            vertex.TexCoords.y = 0.0f;
        }
        vertices.push_back(vertex);
    }
    
    // 处理索引数据
    for (uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i]; // 获取到每个面
        for (uint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // 处理纹理材质数据
    if (mesh->mMaterialIndex != UINT_MAX) { // 一个mesh只有一个材质，所以这里判断当前mesh是否含有材质
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
    Mesh meshObj(vertices, indices, textures);
    return meshObj;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (uint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString iStr;
        mat->GetTexture(type, i, &iStr);
        
        bool skip = false;
        for (uint j = 0; j < textures_loaded.size(); j++) {
            if (strcmp(textures_loaded[j].path.data, iStr.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                cout << "纹理已缓存" << endl;
                break;
            }
        }
        
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(iStr.C_Str(), directory, i);
            texture.type = typeName;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
            cout << "纹理重新加载" << endl;
        }
    }
    return textures;
}

// 加载纹理数据是比较开销比较大的操作，可适当的进行性能优化
uint TextureFromFile(const char *path, const string directory, GLuint textureUnit) {
    string filePath = string(path);
    filePath = directory + '/' + filePath;
    
    uint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, alpha;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &alpha, 0);
    if (data) {
        GLenum format;
        switch (alpha) {
            case 1:
                format = GL_RED;
                break;
                
            case 3:
                format = GL_RGB;
                break;
                
            case 4:
                format = GL_RGBA;
                break;
                
            default:
                format = 0;
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    } else {
        cout << "Texture image load failed." << endl;
        exit(EXIT_SUCCESS);
    }
    
    stbi_image_free(data);
    
    return textureID;
}






