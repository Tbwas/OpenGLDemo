//
//  VertexShader.hpp
//  openGLDemo
//
//  Created by xindong on 2018/1/27.
//  Copyright © 2018年 xindong. All rights reserved.
/**
 在OpenGL中，任何事物都在3D空间中，而屏幕和窗口却是2D像素数组，这导致OpenGL的大部分工作都是关于把3D坐标转变为适应你屏幕的2D像素。3D坐标转为2D坐标的处理过程是由OpenGL的图形渲染管线（Graphics Pipeline，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程）管理的。图形渲染管线可以被划分为两个主要部分：第一部分把你的3D坐标转换为2D坐标，第二部分是把2D坐标转变为实际的有颜色的像素。
 */
#ifndef VertexShader_hpp
#define VertexShader_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

/**
 顶点着色器主要的目的是把3D坐标转为另一种3D坐标(标准化设备坐标)，同时顶点着色器允许我们对顶点属性进行一些基本处理。
 */

class VertexShader {

public:
    // 构造器读取并构建顶点着色器
//    VertexShader(const GLchar *sourceCodePath);
    
    uint32_t createVertexShader(std::string path);
};


#endif
