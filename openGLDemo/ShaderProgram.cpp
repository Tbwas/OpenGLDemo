//
//  ShaderProgram.cpp
//  openGLDemo
//
//  Created by xindong on 2018/3/6.
//  Copyright © 2018年 xindong. All rights reserved.
//

#define GLEW_STATIC // 如果不使用静态链接，就会拷贝一份库文件到应用程序目录
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
    
    /*我们只定义了三个顶点的颜色，原理上那么除这三个顶点所在片段之外，剩下的片段应该是没有任何颜色。而结果并非如此，原因是在片段着色器中进行的所谓片段插值的结果。一个片段一般为一个像素点，也可能包含多个像素点*/
    GLfloat fistVertices[] = {
        -0.5, 0.0, 0.0,  1.0, 0.0, 0.0,  // 第一个顶点包含位置属性和颜色属性
        -0.25, 0.5, 0.0, 0.0, 1.0, 0.0,  // 第二个顶点包含位置属性和颜色属性
        0.0, 0.0, 0.0,   0.0, 0.0, 1.0   // 第三个顶点包含位置属性和颜色属性
    };
    GLfloat secondVertices[] = {
        0.0, 0.0, 0.0,  0.0, 1.0, 1.0,
        0.25, 0.5, 0.0, 1.0, 0.0, 1.0,
        0.5, 0.0, 0.0,  1.0, 1.0, 0.0
    };
    
#pragma mark - 第一个VAO && VBO

    glBindVertexArray(VAOs[0]); // 需要先绑定VAO再绑定顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fistVertices), fistVertices, GL_STATIC_DRAW);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0); // 启用顶点位置属性
    
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GLFW_FALSE, 6 * sizeof(GLfloat), (GLfloat *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // 启动顶点颜色属性
    
#pragma mark - 第二个VAO && VBO
    
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_STATIC_DRAW);
    /**
     告诉OpenGL如何解析顶点数据
     
     @param 0 在顶点着色器源码中，layout(location = 0) 定义了position顶点属性位置，把数据传递到该顶点属性中
     @param 3 顶点属性分量数量（position包含x、y、z共3个分量）
     @param GL_FLOAT 数据类型
     @param GL_FALSE 是否希望数据被标准化
     @param GLfloat 步长，即连续的顶点之间的间隔（或者设为0自动判断）
     @param GLvoid* 表示顶点属性在缓冲中起始位置的偏移量
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLfloat *)0);
    glEnableVertexAttribArray(0); // 启用位置属性，默认禁用
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLfloat *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // 启用颜色属性，默认禁用
    
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
    
    glewExperimental = GL_TRUE;
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
