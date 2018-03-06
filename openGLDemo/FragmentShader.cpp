//
//  FragmentShader.cpp
//  openGLDemo
//
//  Created by xindong on 2018/3/5.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "FragmentShader.hpp"
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

GLuint FragmentShader:: createFragmentShader() {
    
    // 创建着色器对象
    GLuint fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // 从工程目录文件中获取源码字符串
    string shaderString;
    string path = "/Users/momo/Desktop/OpenGL学习/FragmentShader.frag";
    try {
        ifstream ifile(path);
        ostringstream buffer;
        char charactor;
        while (buffer && ifile.get(charactor)) {
            buffer.put(charactor);
        }
        shaderString = buffer.str();
    } catch (exception e) {
        cout << "ERROR: file read error " << &e << endl;
    }
    
    // 编译着色器
    const GLchar *shaderSource = shaderString.c_str();
//    const GLchar *shaderSource = "#version 330 core\n"
//    "out vec4 color;\n"
//    "uniform vec4 ourColor;\n"
//    "void main()\n"
//    "{\n"
//    "color = ourColor;\n"
//    "}\n\0";
    glShaderSource(fragShader, 1, &shaderSource, nullptr);
    glCompileShader(fragShader);
    
    // 检测编译错误
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (success) {
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        cout << "The fragmentShader compile success😊\nThe source code is:\n" << shaderSource << endl;
    } else {
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        cout << "The vertexShader compile error😡" << infoLog << endl;
    }
    
    return fragShader;
}
