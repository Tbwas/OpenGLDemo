//
//  main.cpp
//  openGLDemo
//
//  Created by xindong on 18/1/15.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include <iostream>
#include <glfw3.h>
#include <OpenGL/gl3.h>
#include <cmath>

#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include "ShaderProgram.hpp"
#include "DataSource.hpp"

using namespace std;

#pragma mark - Declaration

GLint textureAlphaLocation;
GLfloat textureAlpha;

void initWindowMakeVisible();
void processInputEvent(GLFWwindow *window, int key, int scanCode, int action, int mods);

#pragma mark - Main

int main(int argc, const char * argv[]) {
    cout << "The current GLFW version is: " << glfwGetVersionString() << endl;
    initWindowMakeVisible();
    return 0;
}

#pragma mark - Implementation

void initWindowMakeVisible() {
    GLFWwindow *window;
    if (!glfwInit()) {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 核心模式
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // 不允许用户调整窗口大小
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 在mac上需要设置, 向前兼容
    
    window = glfwCreateWindow(640, 480, "Hi, OpenGL!  I'm dongxin.", NULL, NULL);
    if (!window) {
        cout << "GLFW create window error" << window << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // 将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    
    cout << "The GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "The OpenGL version is: " << glGetString(GL_VERSION) << endl;
    
    GLint attributeCounts;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributeCounts);
    cout << "顶点着色器中能声明的顶点属性数量上限：" << attributeCounts << endl;
    
    // 我们必须告诉OpenGL渲染窗口的尺寸大小，这样OpenGL才只能知道怎样相对于窗口大小显示数据和坐标
    // OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标(标准化设备坐标)转换为你的屏幕坐标
    int width, height;
    glfwGetFramebufferSize(window, &width , &height);
    glViewport(0, 0, width, height); // 原点位于左下角
    
    // 注册按键回调
    glfwSetKeyCallback(window, processInputEvent);
    
    // 着色器创建、编译、链接、数据装配
    VertexShader vShader;
    GLuint vertexShader = vShader.createVertexShader();
    
    FragmentShader fShader;
    GLuint fragmentShader = fShader.createFragmentShader();
    
    ShaderProgram program;
    GLuint shaderProgram = program.linkVertexShader(vertexShader, fragmentShader);
    if (shaderProgram == -1) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    
    DataSource dataSource;
    GLuint *VAOs = dataSource.setupData();
    glUseProgram(shaderProgram); // 激活程序对象
    
    // 告诉OpenGL每个采样器属于哪个纹理单元
    GLint texture1Location = glGetUniformLocation(shaderProgram, "ourTexture1");
    glUniform1i(texture1Location, 0); // 0为纹理单元GL_TEXTURE0

    GLint texture2Location = glGetUniformLocation(shaderProgram, "ourTexture2");
    glUniform1i(texture2Location, 1); // 1为纹理单元GL_TEXTURE1
    
    textureAlphaLocation = glGetUniformLocation(shaderProgram, "textureAlpha");
    
    while (!glfwWindowShouldClose(window)) {
        
        // 为了避免看见上一次的渲染结果，所以在每次渲染迭代开始时清屏
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区, 之后颜色变为`glClearColor()`所设置的颜色
        
        // 颜色更新, 必须先激活program才能使用uniform
//        GLfloat time = glfwGetTime();
//        GLfloat green = (sin(time) / 2) + 0.5;
//        GLint colorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//        glUniform4f(colorLocation, 0.0, green, 0.0, 1.0);

        // 绘制
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 最后一个参数为偏移量0
        glBindVertexArray(0);

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
    glDeleteVertexArrays(2, VAOs); // 删除VAO
    glfwTerminate();
    exit(EXIT_SUCCESS);
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
    
    if (glfwGetKey(window, key) == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true); // 设置window关闭flag，注意线程安全
                glfwTerminate();
                exit(EXIT_SUCCESS);
                break;
            
            case GLFW_KEY_UP:
                textureAlpha += 0.1;
                textureAlpha = textureAlpha > 1.0 ? 1.0 : textureAlpha;
                glUniform1f(textureAlphaLocation, textureAlpha);
                break;
                
            case GLFW_KEY_DOWN:
                textureAlpha -= 0.1;
                textureAlpha = textureAlpha < 0.0 ? 0.0 : textureAlpha;
                glUniform1f(textureAlphaLocation, textureAlpha);
                break;
                
            default:
                break;
        }
    }
}

