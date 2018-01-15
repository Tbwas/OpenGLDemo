//
//  main.cpp
//  openGLDemo
//
//  Created by xindong on 18/1/15.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include <iostream>
#include <glfw3.h>

void Render();

int main(int argc, const char * argv[]) {
    
    GLFWwindow *window;
    if (!glfwInit()) {
        return -1;
    }
    window = glfwCreateWindow(640, 480, "Hi, OpenGL!  I'm dongxin.", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
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
