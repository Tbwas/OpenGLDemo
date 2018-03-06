//
//  ShaderProgram.cpp
//  openGLDemo
//
//  Created by xindong on 2018/3/6.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include <glew.h>
#include "ShaderProgram.hpp"
#include <glfw3.h>
#include <iostream>

using namespace std;

// VAO和VBO还可以是数组
GLuint* ShaderProgram:: setupVertextData() {
    
    static GLuint VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs); // 创建VAO，缓存数量为2
    glGenBuffers(2, VBOs); // 创建VBO，缓存数量为2
    
    GLfloat fistVertices[] = {
        -0.5, 0.0, 0.0,
        -0.25, 0.5, 0.0,
        0.0, 0.0, 0.0
    };
    GLfloat secondVertices[] = {
        0.0, 0.0, 0.0,
        0.25, 0.5, 0.0,
        0.5, 0.0, 0.0,
    };
    
    // MARK: - 第一个VAO && VBO
    glBindVertexArray(VAOs[0]); // 需要先绑定VAO再绑定顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fistVertices), fistVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0);
    
    // MARK: - 第二个VAO && VBO
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_STATIC_DRAW);
    /**
     告诉OpenGL如何解析顶点数据
     
     @param 0 在顶点着色器源码中，layout(location = 0) 定义了position顶点属性位置，把数据传递到该顶点属性中
     @param 3 顶点属性分量数量（position包含x、y、z共3个分量）
     @param GL_FLOAT 数据类型
     @param GL_FALSE 是否希望数据被标准化
     @param GLfloat 步长，即连续的顶点属性之间的间隔（或者设为0自动判断）
     @param GLvoid* 表示位置数据在缓冲中起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0); // 顶点属性默认是禁用，所以这里要启用
    
    // VAO会储存glBindBuffer的函数调用, 也就是说，如果在解绑VAO之前解绑了VBO或者EBO，那么VAO会保存解绑操作，之前的所有绑定操作都白玩了.
    glBindVertexArray(0); // 解绑VAO
//    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO, 貌似不需要该步骤
    glDeleteBuffers(2, VBOs); // 删除VBO
    
    return VAOs;
}

GLuint ShaderProgram:: linkVertexShader(GLuint vertexShader, GLuint fragmentShader) {
    if (!vertexShader || !fragmentShader) {
        cout << "shader cann't be nil" << vertexShader << fragmentShader << endl;
        return -1;
    }
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cout << "glewInit error:" << err << endl;
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
