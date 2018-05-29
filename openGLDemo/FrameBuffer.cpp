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

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &FBOID);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOID); // 包括`GL_READ_FRAMEBUFFER`和`GL_DRAW_FRAMEBUFFER`

    // (1) 创建纹理作为frameBuffer的<颜色附件>
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // NULL表示仅仅分配内存却没有填充数据
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // (2) 创建渲染缓冲对象作为frameBuffer的<深度附件>
    GLuint RBOID;
    glGenRenderbuffers(1, &RBOID);
    glBindRenderbuffer(GL_RENDERBUFFER, RBOID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // 创建一个深度和模板渲染缓冲对象为了进行深度测试
    glBindRenderbuffer(GL_RED_INTEGER, 0); // 分配内存之后便可以解绑
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOID);
    
    // 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        printf("执行胜利的舞蹈");
    } else {
        printf("当前创建的帧缓冲不完整");
        abort();
    }
    
    /** 现在这个帧缓冲就完整了，我们只需要绑定这个帧缓冲对象，让渲染到帧缓冲的缓冲中而不是默认的帧缓冲中。**/
    glDeleteFramebuffers(1, &FBOID); // 完成所有操作后删除帧缓冲对象
}
