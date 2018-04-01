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

static void __unused MatrixTransform() {
    glm::vec4 vec(1.0, 0.0, 0.0, 1.0); // 定义向量vec
    glm::mat4 trans; // 定义一个4*4的单位矩阵

    // 通过单位矩阵和位移向量来创建一个变换矩阵
    // 即Tx = 2.0, Ty = 3.0, Tz = 4.0
    trans = glm::translate(trans, glm::vec3(2.0, 3.0, 3.0));

    // 变换矩阵和向量相乘即可获得最终结果
    vec = trans * vec;
    cout << vec.x << vec.y << vec.z << endl;
}

GLuint* DataSource:: setupData() {
    
//    MatrixTransform();
    
    // @Note 书写数据时一定要注意纹理坐标系和OpenGL坐标系的映射关系
//    GLfloat datas[] = {
//        /*位置属性*/       /*颜色属性*/    /*纹理属性*/
//        -0.5, 0.5, 0.0,  0.0, 1.0, 1.0,  0.0, 1.0,
//        0.5, 0.5, 0.0,   1.0, 0.0, 1.0,  1.0, 1.0,
//        0.5, -0.5, 0.0,  1.0, 1.0, 0.0,  1.0, 0.0,
//        -0.5, -0.5, 0.0, 1.0, 0.0, 0.0,  0.0, 0.0
//    };
    
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
    
    // 绘制矩形要借助EBO，这样才能避免额外开销
//    GLuint indexs[] = {
//        0, 1, 2, // 第一个三角形
//        2, 3, 0  // 第二个三角形
//    };
    
    // VAO
    static GLuint VAOs[1];
    glGenVertexArrays(1, VAOs);
    glBindVertexArray(VAOs[0]);
    
    // VBO
    GLuint VBOs[1];
    glGenBuffers(1, VBOs);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(datas), datas, GL_STATIC_DRAW); // 把数据拷贝到缓冲中
    
    // EBO
//    GLuint EBOs[1];
//    glGenBuffers(1, EBOs);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLfloat *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    /**
    // 位置属性数据解析方式
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0);
    
    // 颜色属性数据解析方式
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLfloat *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    // 纹理属性数据解析方式
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLfloat *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
     */

    // 纹理对象
    GLuint ourTexture1;
    glGenTextures(1, &ourTexture1);
    
    // 纹理单元0默认为激活
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ourTexture1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // s轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // t轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时的过滤方式
    
    // 使用图片数据生成纹理并保存在纹理对象中
    int width, height, alpha;
    unsigned char *imageData = stbi_load("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/container.jpg", &width, &height, &alpha, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D); // 生成多级渐远纹理
    } else {
        cout << "Failed to load texture from image" << endl;
        exit(EXIT_SUCCESS);
    }
    stbi_image_free(imageData); // 释放图片数据
    
    // 第二个纹理对象
    GLuint ourTexture2;
    glGenTextures(1, &ourTexture2);
    
    // 激活纹理单元1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ourTexture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小时的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大时的过滤方式
    
    stbi_set_flip_vertically_on_load(true); // 图片竖直翻转
    imageData = stbi_load("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/cute.jpg", &width, &height, &alpha, 0);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    } else {
        cout << "Failed to load texture from image" << endl;
        exit(EXIT_SUCCESS);
    }
    stbi_image_free(imageData);
    
    // 解绑AVO
    glBindVertexArray(0);
    glDeleteBuffers(1, VBOs);
//    glDeleteBuffers(1, EBOs);
    return VAOs;
}
