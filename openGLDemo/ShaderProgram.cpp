//
//  ShaderProgram.cpp
//  openGLDemo
//
//  Created by xindong on 2018/3/6.
//  Copyright © 2018年 xindong. All rights reserved.
//

#define GLEW_STATIC // 如果不使用静态链接，就会拷贝一份库文件到应用程序目录
#include <OpenGL/gl3.h>
#include "ShaderProgram.hpp"
#include <iostream>
#include "stb_image.h"

using namespace std;


#pragma mark -
/*=========================华丽的分割线==================================*/
#pragma mark -

// VAO和VBO还可以是数组
GLuint* setupVertextData() {
    
    static GLuint VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs); // 创建VAO，缓存数量为2
    glGenBuffers(2, VBOs); // 创建VBO，缓存数量为2
    
    
    /*我们只定义了三个顶点的颜色，原理上那么除这三个顶点所在片段之外，剩下的片段应该是没有任何颜色。而结果并非如此，原因是在片段着色器中进行的所谓片段插值的结果。一个片段一般为一个像素点，也可能包含多个像素点*/
    GLfloat fistVertices[] = {
        -0.5, 0.0, 0.0,  1.0, 0.0, 0.0,  // 第一个顶点包含位置属性和颜色属性
        -0.25, 0.5, 0.0, 0.0, 1.0, 0.0,  // 第二个顶点包含位置属性和颜色属性
        0.0, 0.0, 0.0,   0.0, 0.0, 1.0   // 第三个顶点包含位置属性和颜色属性
    };
    GLfloat secondVertices[] = {
        /*位置属性*/       /*颜色属性*/    /*纹理属性*/
        0.0, 0.0, 0.0,  0.0, 1.0, 1.0,  0.0, 0.0,
        0.5, 1.0, 0.0,  1.0, 0.0, 1.0,  0.25, 0.5,
        1.0, 0.0, 0.0,  1.0, 1.0, 0.0,  0.5, 0.0
    };
    
/** 第一个VAO && VBO **/

    glBindVertexArray(VAOs[0]); // 需要先绑定VAO再绑定顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fistVertices), fistVertices, GL_STATIC_DRAW);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0); // 启用顶点位置属性
    
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLfloat *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // 启动顶点颜色属性
    
/** 第二个VAO && VBO **/
    
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_STATIC_DRAW);
    
    // 创建纹理对象并绑定类型
    GLuint texture[1];
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t轴环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小时的过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大时的过滤方式
    
    // 使用图片数据生成纹理
    int width, height, alpha;
    unsigned char *data = stbi_load("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/container.jpg", &width, &height, &alpha, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // 生成多级渐远纹理
    } else {
        cout << "Failed to load texture from image" << endl;
        exit(EXIT_SUCCESS);
    }
    stbi_image_free(data); // 释放图片数据

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0); // 启用位置属性，默认禁用
    
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLfloat *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // 启用颜色属性，默认禁用
    
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLfloat *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    /**VAO会储存glBindBuffer的函数调用, 也就是说，如果在解绑VAO之前解绑了VBO或者EBO，那么VAO会保存解绑操作，之前的所有绑定操作都白玩了**/
    glBindVertexArray(0); // 解绑VAO
    glDeleteBuffers(2, VBOs); // 删除VBO
    
    return VAOs;
}

GLuint ShaderProgram:: linkVertexShader(GLuint vertexShader, GLuint fragmentShader) {
    if (!vertexShader || !fragmentShader) {
        cout << "shader cann't be nil" << vertexShader << fragmentShader << endl;
        return -1;
    }
    
    // 把着色器附加到程序上，然后链接它们
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // 链接错误检测
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success) {
        cout << "The shader link success😊" << endl;
    } else {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        cout << "The shader link error😡: " << infoLog << endl;
        return -1;
    }
    
    // 删除着色器对象，以后不再需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}
