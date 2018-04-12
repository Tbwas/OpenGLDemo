//
//  FragmentShader.cpp
//  openGLDemo
//
//  Created by xindong on 2018/3/5.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "FragmentShader.hpp"
#include <OpenGL/gl3.h>
#include <fstream>
#include <sstream>

using namespace std;

GLuint FragmentShader:: createFragmentShader() {
    
    string fShaderString;
    string fShaderPath = "/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/FragmentShader.frag";
    ifstream fileStream;
    ostringstream strStream;
    
    // 保证文件流对象可以抛出异常
    fileStream.exceptions(ifstream::badbit);
    
    try {
        fileStream.open(fShaderPath); // 打开文件
        strStream << fileStream.rdbuf(); // 读取文件内容到字符串流
        fileStream.close(); // 关闭文件
    } catch (ifstream::failure e) {
        cout<< "ERROR: file read error" << &e <<endl;
    }
    
    fShaderString = strStream.str();
    const GLchar *shaderSource = fShaderString.c_str();
    
    // 创建着色器对象并编译
    GLuint fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
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
        exit(EXIT_SUCCESS);
    }
    
    return fragShader;
}
