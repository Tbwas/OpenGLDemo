//
//  DataSource.cpp
//  openGLDemo
//
//  Created by xindong on 2018/3/8.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include <OpenGL/gl3.h>
#include "DataSource.hpp"
#include <iostream>
#include "stb_image.h"

/** 以下3个头文件可满足大多数矩阵变换运算功能 **/
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

GLuint DataSource:: setupData() {
    
    // @Note 书写数据时一定要注意纹理坐标系和OpenGL坐标系的映射关系
    float datas[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
  
    // VAO
    static GLuint VAOID;
    glGenVertexArrays(1, &VAOID);
    glBindVertexArray(VAOID);
    
    // VBO
    GLuint VBOID;
    glGenBuffers(1, &VBOID); // 返回n个当前未使用的缓存对象名称（类似C语言中的指针变量），并保存到VBOs数组中，这n个名称是无符号整型数据，但不一定是连续的数据。此时这些名称仅仅只是名字而已，还不是真正的缓存对象，只有这些名称首次绑定到OpenGL上下文中的目标上之后，它所对应的缓存对象才会被真正的创建出来。
    glBindBuffer(GL_ARRAY_BUFFER, VBOID); // 将缓存对象名称绑定到目标`GL_ARRAY_BUFFER`上，如果是第一次绑定，则会创建一个与它对应的缓存对象。
    glBufferData(GL_ARRAY_BUFFER, sizeof(datas), datas, GL_STATIC_DRAW); // 把数据传输到缓存对象中
    
    // 位置属性数据解析方式
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    
    // 纹理属性指针
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLfloat *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    
    // 纹理
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // s轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // t轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时的过滤方式
    
    // 使用图片数据生成纹理并保存在纹理对象中
    int width, height, alpha;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/lightBox.jpg", &width, &height, &alpha, 0);
    if (imageData) {
        // 将图像数据被添加到纹理对象ourTexture1上
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D); // 生成多级渐远纹理
    } else {
        cout << "Failed to load texture from image" << endl;
        exit(EXIT_SUCCESS);
    }
    stbi_image_free(imageData); // 释放图片数据

    glBindVertexArray(0);
    return VAOID;
}

unsigned char * DataSource::getImageData(const char *path) {
    int width, height, alpha;
    stbi_set_flip_vertically_on_load(true);
   unsigned char *imageData = stbi_load(path, &width, &height, &alpha, 0);
    if (imageData) {
        return imageData;
    }
    cout << "Failed to load texture from image" << endl;
    return nullptr;
}











































