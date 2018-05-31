//
//  FrameBuffer.cpp
//  openGLDemo
//
//  Created by xindong on 2018/4/27.
//  Copyright © 2018年 xindong. All rights reserved.
//

#include "FrameBuffer.hpp"
#include <iostream>
#include "DataSource.hpp"
#include <string>
#include "stb_image.h"

void FrameBuffer:: GenerateFBO(unsigned int width, unsigned int height) {
    glGenFramebuffers(1, &FBOID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
    
    // color attachment
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // only allocate memory (must do this).
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0); // 将纹理绑定到颜色缓冲区
    
    /*@Note: 如果texture attachment绑定到frameBuffer中的`GL_COLOR_ATTACHMENT0`, 则下面这两行代码无需调用, 默认情况下在colorBuffer0中绘制.
             如果绑定到其他值`GL_COLOR_ATTACHMENT.n`, 则下面两行代码必须调用, 并且数组drawBuffer中的元素必须是对应的`GL_COLOR_ATTACHMENT.n`*/
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers); // 将fragmentShader输出的内容绘制到frameBuffer中指定的colorBuffer中
    
    // depth attachment
    glGenRenderbuffers(1, &RBOID);
    glBindRenderbuffer(GL_RENDERBUFFER, RBOID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); // only allocate memory (must do this).
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBOID);
    
    // check it is completed
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        glBindRenderbuffer(GL_FRAMEBUFFER, 0);
        printf("执行胜利的舞蹈");
    } else {
        printf("当前创建的帧缓冲不完整");
        abort();
    }
}

void FrameBuffer:: bindFBO(GLuint width, GLuint height) {
    glBindFramebuffer(GL_FRAMEBUFFER, FBOID);
    glViewport(0, 0, width, height);
}

void FrameBuffer:: unBindFBO(GLuint width, GLuint height) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
}














