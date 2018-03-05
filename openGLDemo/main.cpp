//
//  main.cpp
//  openGLDemo
//
//  Created by xindong on 18/1/15.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include <fstream>

#include "VertexShader.hpp"

using namespace std;

#pragma mark - Declaration

void initWindowMakeVisible();
void Render();
void processInputEvent(GLFWwindow *window, int key, int scanCode, int action, int mods);

#pragma mark - Main

int main(int argc, const char * argv[]) {
    initWindowMakeVisible();
    return 0;
}

#pragma mark - Implementation

void initWindowMakeVisible() {
    GLFWwindow *window;
    if (!glfwInit()) {
        return;
    }
    window = glfwCreateWindow(640, 480, "Hi, OpenGL!  I'm dongxin.", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window); // 将我们窗口的上下文设置为当前线程的主上下文
    
    // 我们必须告诉OpenGL渲染窗口的尺寸大小，这样OpenGL才只能知道怎样相对于窗口大小显示数据和坐标
    // OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标(标准化设备坐标)转换为你的屏幕坐标
    int width, height;
    glfwGetFramebufferSize(window, &width , &height);
    glViewport(0, 0, width, height); // 原点位于左下角
    
    glfwSetKeyCallback(window, processInputEvent); // 注册按键回调
    
    createVertexShader();
    
    while (!glfwWindowShouldClose(window)) {
        Render();
        
        /**
         应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。 这是因为生成的图像不是一下子被绘制出来
         的，而是按照从左到右，由上而下逐像素地绘制而成的。最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会
         导致渲染的结果很不真实。为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在
         屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓
         冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
         */
        glfwSwapBuffers(window); // 颜色缓冲区存储着GLFW窗口每一个像素颜色
        glfwPollEvents(); // 监听事件
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Render(void) {
    
    // 为了避免看见上一次的渲染结果，所以在每次渲染迭代开始时清屏
    glClearColor(255.0, 255.0, 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区, 之后颜色变为`glClearColor()`所设置的颜色
    
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0, 0.5);
        
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(-0.5, -0.5);
        
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(0.5, -0.5);
    }
    glEnd();
}


/**
 按键回调
 
 @param window 窗口
 @param key 被按下的键
 @param scanCode 按键的系统扫描代码
 @param action 被按下还是被释放
 @param mods 表示是否有Ctrl、Shift、Alt等按钮操作
 */
void processInputEvent(GLFWwindow *window, int key, int scanCode, int action, int mods) {
    cout<< "key is:" << key <<endl;
    cout<< "scanCode is:" << scanCode <<endl;
    cout<< "action is:" << action <<endl;
    cout<< "mods is:" << mods <<endl;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, true); // 设置window关闭flag，注意线程安全
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    
}











































