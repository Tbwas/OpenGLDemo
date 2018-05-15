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

using namespace std;
using namespace glm;

#pragma mark - Declaration

GLint textureAlphaLocation;
GLfloat textureAlpha;

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


void initWindowMakeVisible();
void processInputEvent(GLFWwindow *window, int key, int scanCode, int action, int mods);
void mouseCallback(GLFWwindow *window, double xPos, double yPos);

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

    // 注册按键回调
    glfwSetKeyCallback(window, processInputEvent);
    
    // 不显示光标并且捕捉它
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // 监听鼠标移动事件
    glfwSetCursorPosCallback(window, mouseCallback);
    
    
/*---------------------------------华丽的分割线----------------------------------------------------*/

    // 线上回滚测试 ***
    
    // 顶点着色器
    VertexShader vShader;
    GLuint vertexShader = vShader.createVertexShader();
    
    // 片元着色器
    FragmentShader fShader;
    GLuint fragmentShader = fShader.createFragmentShader("/Users/momo/Desktop/OpenGLDemo/openGLDemo/Resources/FragmentShader.frag");
    GLuint allShaders[] = {vertexShader, fragmentShader};
    
    // 着色器程序对象
    ShaderProgram program;
    GLuint shaderProgram = program.linkShaders(allShaders);
    if (shaderProgram == -1) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    glUseProgram(shaderProgram);

    // 数据装配
    DataSource dataSource;
    GLuint VAOID = dataSource.setupData();
    
    // 帧缓冲配置
    FrameBuffer frameBuffer;
    
    // 创建一个投影矩阵 - @!!!: Note 需要通过该矩阵将输入坐标转为3D标准化设备坐标.
    mat4 projection(1.0f);
    projection = perspective(radians(45.0f), (float)width / height, 0.1f, 100.0f); // 投影矩阵参数通常这样配置
    
    // 纹理采样器配置
    glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);
    
    while (!glfwWindowShouldClose(window)) {
        
        GLfloat currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.FBOID);
        glEnable(GL_DEPTH_TEST); // 启用深度测试
        
        // 绘制物体
        glBindVertexArray(VAOID);

        float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        
        mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        GLuint modelLoca = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoca, 1, GL_FALSE, value_ptr(model));

        mat4 view(1.0f);
        view = glm::translate(view, vec3(0.0, 0.0, -3.0f));
        GLuint viewLoca = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoca, 1, GL_FALSE, value_ptr(view));
        
        GLuint projeLoca = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projeLoca, 1, GL_FALSE, value_ptr(projection));
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window); // 颜色缓冲区存储着GLFW窗口每一个像素颜色
        glfwPollEvents(); // 监听事件
        
    }
    glDeleteVertexArrays(1, &VAOID);
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
    
    float camSpeed = 2.5f * deltaTime;
    
    if (glfwGetKey(window, key) == GLFW_PRESS) {
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
                
            case GLFW_KEY_W:
                camPosition -= camSpeed * camDirection;
                break;
                
            case GLFW_KEY_S:
                camPosition += camSpeed * camDirection;
                break;
                
            case GLFW_KEY_A:
                camPosition -= cross(camDirection, camUp) * camSpeed;
                break;
                
            case GLFW_KEY_D:
                camPosition += cross(camDirection, camUp) * camSpeed;
                break;
                
            default:
                break;
        }
    }
}


/**
 鼠标移动回调

 @param window 窗口
 @param xPos x位置
 @param yPos y位置
 */
void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    return;
    
    cout << "鼠标在移动, x: " << xPos << " y: " << yPos << endl;
    
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // 这里是相反的，因为鼠标向下移动yPos是增大的，而我们希望是鼠标向上移动增大的
    lastX = xPos;
    lastY = yPos;
    
    float sensitivity = 0.5;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    pitchAngle += yOffset;
    yawAngle += xOffset;
    
    // 做个大小限制
    pitchAngle = pitchAngle > 89.0f ? 89.0f : pitchAngle < -89.0f ? -89.0 : pitchAngle;
    
    // 通过俯仰角和偏航角来计算方向向量
    vec3 direction(1.0f);
    direction.x = -cos(radians(pitchAngle)) * cos(radians(yawAngle));
    direction.y = -sin(radians(pitchAngle));
    direction.z = -cos(radians(pitchAngle)) * sin(radians(yawAngle));
    camDirection = direction;
    
}

