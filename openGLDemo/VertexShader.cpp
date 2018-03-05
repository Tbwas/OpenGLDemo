//
//  VertexShader.cpp
//  openGLDemo
//
//  Created by xindong on 2018/1/27.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "VertexShader.hpp"
#include <glew.h>
#include <glfw3.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class VertexShader {
    // instance variables
    
public:
    
    // 每个顶点是一个3D坐标，而存放顶点的数组叫做顶点数据
    void setupVertexData() {
        GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
        
        // 顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存(通常被称为显存)中储存大量顶点。
        GLuint VBO; // 缓冲ID
        glGenBuffers(1, &VBO); // 创建VBO对象, 1表示需要创建的缓存数量
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定缓冲类型，之后我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
        
        // 有了上述绑定操作之后，就可以将数据复制到VBO管理的内存中
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 至此，我们已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理
    }
    
    
    void createVertexShader() {
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            cout << "glewInit error:" << err << endl;
            return;
        }
        
        // 创建着色器对象
        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        
        // 从文件路径获得vertex源码
        string vertexString;
        char data[1024];
        const GLchar *vertexFilePath = "/Users/momo/Desktop/OpenGL学习/VertextShader.vert";
        try {
            // 以读的方式打开文件
            ifstream infile;
            infile.open(vertexFilePath);
            infile >> data;
            vertexString = data;
        } catch (exception e) {
            cout<< "ERROR: file read error" << &e <<endl;
        }
        
        // 编译顶点着色器
        const GLchar *vShaderCode = vertexString.c_str();
        glShaderSource(vertexShader, 1, &vShaderCode, NULL); // 1为源码字符串数量
        glCompileShader(vertexShader);
        
        // 检测是否编译成功
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            cout << "😒vertexShader compile error: " << infoLog << endl;
        } else {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            cout << "☺️vertexShader compile success: " << vShaderCode << endl;
        }
    }

    
};

