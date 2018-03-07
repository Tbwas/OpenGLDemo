//
//  VertexShader.cpp
//  openGLDemo
//
//  Created by xindong on 2018/1/27.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "VertexShader.hpp"

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

