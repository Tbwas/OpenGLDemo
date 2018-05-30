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

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include "ShaderProgram.hpp"
#include "DataSource.hpp"

#include "FrameBuffer.hpp"

#define DX_INLINE static __attribute__((always_inline))

using namespace std;
using namespace glm;

#pragma mark - Declaration Variables

GLint textureAlphaLocation;
GLfloat textureAlpha;
mat4 projection(1.0f);
FrameBuffer frameBuffer;
DataSource dataSource;

VertexShader vShader;
FragmentShader fShader;
ShaderProgram program;

// 摄像机相关配置
vec3 camPosition = vec3(0.0f, 0.0, 3.0f);    // 摄像机位置向量
vec3 camDirection = vec3(0.0f, 0.0f, -1.0f); // 摄像机方向向量
vec3 camUp = vec3(0.0f, 1.0f, 0.0f); // 向上向量

// 光源的相关配置
vec3 lightColor(1.0f, 1.0f, 1.0f);
vec3 lightPosition(0.1, 0.5, -2.0);
vec3 lightAmbient(0.2f, 0.2f, 0.2f);
vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
vec3 lightSpecular(1.0f, 1.0f, 1.0f);

// 物体材质的相关配置
vec3 materialAmbient(1.0f, 0.5f, 0.31f);
vec3 materialDiffuse(1.0f, 0.5f, 0.31f);
vec3 materialSpecular(0.5f, 0.5f, 0.5f);

GLfloat deltaTime = 0.0f; // 当前帧与上一帧绘制的时间差，用来平衡不同硬件间的移动速度
GLfloat lastTime = 0.0f;

// 鼠标配置相关
GLfloat lastX = 320.0f; // 设置鼠标初始位置
GLfloat lastY = 240.0f; // 设置鼠标初始位置
GLfloat pitchAngle = 0.0f; // 俯仰角
GLfloat yawAngle = 0.0f; // 偏航角

#pragma mark - Declaration Functions

void initWindowMakeVisible();
void processInputEvent(GLFWwindow *window, int key, int scanCode, int action, int mods);
void mouseCallback(GLFWwindow *window, double xPos, double yPos);

DX_INLINE void startToDisplay1(GLuint VAOID, GLuint program);
DX_INLINE void startToDisplay2(GLuint VAOID, GLuint program);

/**
 应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。 这是因为生成的图像不是一下子被绘制出来
 的，而是按照从左到右，由上而下逐像素地绘制而成的。最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会
 导致渲染的结果很不真实。为了规避这些问题，我们应用双缓冲渲染窗口应用程序。前缓冲保存着最终输出的图像，它会在
 屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓
 冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
 */
GLFWAPI void glfwSwapBuffers(GLFWwindow* window);

/**
 @param location uniform变量位置值
 @param count 矩阵数量，即要发送多少个矩阵到shader
 @param transpose 矩阵是否置换，即交换矩阵的行和列，一般不需要
 @param value 真正的矩阵数据
 */
extern void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);


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
    
    window = glfwCreateWindow(640, 480, "心董儿", NULL, NULL);
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
    
/*---------------------------------华丽的分割线----------------------------------------------------*/

    
    // 顶点着色器
    GLuint vertexShader = vShader.createVertexShader("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/VertextShader.vert");
    GLuint fbVShader = vShader.createVertexShader("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/frameBufferVShader.vert");
    
    // 片元着色器
    GLuint fragmentShader = fShader.createFragmentShader("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/FragmentShader.frag");
    GLuint fbFShader = fShader.createFragmentShader("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/frameBufferFShader.frag");

    // 着色器程序对象
    GLuint allShaders[2] = {vertexShader, fragmentShader};
    GLuint shaderProgram = program.linkShaders(allShaders);
    if (shaderProgram == -1) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    
    GLuint fbShaders[2] = {fbVShader, fbFShader};
    GLuint fbProgram = program.linkShaders(fbShaders);
    if (fbProgram == -1) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    // 数据装配
    GLuint VAOID = dataSource.setupData();
    GLuint fbVAOID = dataSource.quadVAO();
    
    // 创建一个投影矩阵 - @!!!: Note 需要通过该矩阵将输入坐标转为3D标准化设备坐标.
    projection = perspective(radians(45.0f), (float)width / height, 0.1f, 100.0f); // 投影矩阵参数通常这样配置
    
    frameBuffer.GenerateFBO(width, height);
    
    while (!glfwWindowShouldClose(window)) {
        
        GLfloat currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        /** PASS 1 **/
        startToDisplay1(VAOID, shaderProgram);
        
        /** PASS 2 **/
        startToDisplay2(fbVAOID, fbProgram);
        
        // @Tips: PASS1禁用深度测试, PASS2开启深度测试, 结果发现深度测试没有生效, 说明PASS2绘制的内容是从我们创建的frameBuffer读取的纹理数据, 即创建的frameBuffer成功.

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    glDeleteVertexArrays(1, &VAOID);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

DX_INLINE void startToDisplay1(GLuint VAOID, GLuint program) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.FBOID);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    glUseProgram(program);
    glEnable(GL_DEPTH_TEST);
    
    mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    GLuint modelLoca = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoca, 1, GL_FALSE, value_ptr(model));
    
    mat4 view(1.0f);
    view = glm::translate(view, vec3(0.0, 0.0, -3.0f));
    GLuint viewLoca = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(viewLoca, 1, GL_FALSE, value_ptr(view));
    
    GLuint projeLoca = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projeLoca, 1, GL_FALSE, value_ptr(projection));
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dataSource.textureID);
    glUniform1i(glGetUniformLocation(program, "renderTexture"), 0);
    
    glBindVertexArray(VAOID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

DX_INLINE void startToDisplay2(GLuint VAOID, GLuint program) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.5, 0.1, 1.0);
    
    glUseProgram(program);
    glDisable(GL_DEPTH_TEST);
    
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, frameBuffer.textureID);
    glUniform1i(glGetUniformLocation(program, "textureSampler"), 1);
    
    glBindVertexArray(VAOID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
}






















