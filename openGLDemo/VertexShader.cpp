//
//  VertexShader.cpp
//  openGLDemo
//
//  Created by xindong on 2018/1/27.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "VertexShader.hpp"
#include <OpenGL/gl3.h>

using namespace std;

/**
VertexShader:: VertexShader(const GLchar *sourceCodePath) {
    string vShaderString;
    ifstream vShaderFile;
    
    // 保证ifstream对象可以抛出异常
    vShaderFile.exceptions(ifstream::badbit);
    
    try {
        // 使用文件流打开文件
        vShaderFile.open(sourceCodePath);
        
        // 使用字符串流读取文件的缓冲内容到流中
        stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        
        // 关闭文件
        vShaderFile.close();
    } catch (ifstream::failure e) {
        cout << "The vertexShader file read error" << endl;
    }
    
    const GLchar *vShaderCode = vShaderString.c_str();
    
    // 创建着色器对象并编译
    GLuint vertexShader(glCreateShader(GL_VERTEX_SHADER));
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
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
        cout << "The vertexShader compile success😊\nThe source code is:\n" << vShaderCode << endl;
    }
}
 */

GLuint VertexShader:: createVertexShader(std::string path) {
    
    // 从文件路径获得vertex源码
    ifstream fileStream;
    ostringstream strStream;
    string vShaderString;
    string vShaderPath = path;
    
    // 保证文件流对象可以抛出异常
    fileStream.exceptions(ifstream::badbit);
    
    try {
        fileStream.open(vShaderPath); // 打开文件
        strStream << fileStream.rdbuf(); // 读取文件缓冲内容到字符串流中
        fileStream.close(); // 关闭文件
    } catch (ifstream::failure e) {
        cout<< "ERROR: file read error" << &e <<endl;
    }
    
    vShaderString = strStream.str(); // 必须通过string类型变量中转
    const GLchar *vShaderCode = vShaderString.c_str();

    // 创建着色器对象并编译
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL); // 1为源码字符串数量
    glCompileShader(vertexShader);
    
    // 检测是否编译成功
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "The vertexShader compile error😡" << infoLog << endl;
        exit(EXIT_SUCCESS);
    } else {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "The vertexShader compile success😊\nThe source code is:\n" << vShaderCode << endl;
    }
    
    return vertexShader;
}
