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

GLuint ShaderProgram:: setupVertextData() {
    
    // 创建VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // 需要先绑定VAO再绑定顶点数据
    
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // position左下
        0.5f, -0.5f, 0.0f,  // position右下
        0.0f,  0.5f, 0.0f   // position顶部
    };
    
    // 顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存(通常被称为显存)中储存大量顶点
    GLuint VBO;
    glGenBuffers(1, &VBO); // 创建VBO对象, 1表示需要创建的缓存数量
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定缓冲类型，之后我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
    // 有了上述绑定操作之后，就可以将数据复制到VBO管理的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /** 至此，我们已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理 **/
    
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
    
    // 解绑VAO
    glBindVertexArray(0);
    
    return VAO;
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
