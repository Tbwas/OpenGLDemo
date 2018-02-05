//
//  main.cpp
//  openGLDemo
//
//  Created by xindong on 18/1/15.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include <iostream>
#include <glfw3.h>

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
    
    glfwMakeContextCurrent(window);
    
    glViewport(0, 0, 200, 200);
    glfwSetKeyCallback(window, processInputEvent);
    
    while (!glfwWindowShouldClose(window)) {
        Render();
        glfwSwapBuffers(window); // 交换前后缓冲区数据
        glfwPollEvents(); // 监听事件
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Render(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
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

void processInputEvent(GLFWwindow *window, int key, int scanCode, int action, int mods) {
    cout<< "key is:" << key <<endl;
    cout<< "scanCode is:" << scanCode <<endl;
    cout<< "action is:" << action <<endl;
    cout<< "mods is:" << mods <<endl;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, true);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    
}











































