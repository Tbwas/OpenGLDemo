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

GLuint ShaderProgram:: linkShaders(GLuint *shaderArray) {
    if (!shaderArray) {
        cout << "shader cann't be nil" << endl;
        return -1;
    }
    
    // 把着色器附加到程序上，然后链接它们
    GLuint program = glCreateProgram();
    int shaderCount = sizeof(shaderArray) / sizeof(GLuint);
    
    for (int i = 0; i < shaderCount; i++) {
        glAttachShader(program, shaderArray[i]);
    }
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
    for (int i = 0; i < shaderCount; i++) {
        glDeleteShader(shaderArray[i]);
    }
    
    return program;
}

