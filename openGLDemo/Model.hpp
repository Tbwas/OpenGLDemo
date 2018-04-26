//
//  Model.hpp
//  openGLDemo
//
//  Created by xindong on 2018/4/25.
//  Copyright © 2018年 xindong. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>
#include "Mesh.hpp"

class Model {
    
public:
    Model(string path) {
        loadModel(path);
    }
    void Draw(GLuint shaderProgram);
    
private:
    /* 模型数据 */
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif
