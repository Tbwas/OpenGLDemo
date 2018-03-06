//
//  VertexShader.cpp
//  openGLDemo
//
//  Created by xindong on 2018/1/27.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "VertexShader.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glfw3.h>

using namespace std;

GLuint VertexShader:: createVertexShader() {
    
    // 创建着色器对象
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    // 从文件路径获得vertex源码
    string vertexString;
    string vertexFilePath = "/Users/momo/Desktop/OpenGL学习/VertextShader.vert";
    try {
        ifstream ifile(vertexFilePath);
        ostringstream buffer; // 父类：ostream、stringstream
        char charactor;
        while (buffer && ifile.get(charactor)) {
            buffer.put(charactor);
        }
        vertexString = buffer.str();
        
    } catch (exception e) {
        cout<< "ERROR: file read error" << &e <<endl;
    }
    
    // 编译顶点着色器
    const GLchar *vertexShaderSource = vertexString.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 1为源码字符串数量
    glCompileShader(vertexShader);
    
    // 检测是否编译成功
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "The vertexShader compile error😡" << infoLog << endl;
    } else {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "The vertexShader compile success😊\nThe source code is:\n" << vertexShaderSource << endl;
    }
    
    return vertexShader;
}

