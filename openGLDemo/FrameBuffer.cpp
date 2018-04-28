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
  /*glBindFramebuffer(GL_FRAMEBUFFER, 0); 绑定到系统默认缓冲*/
    
    // 添加一些附件到帧缓冲中, 之后所有的渲染操作将会渲染到当前绑定帧缓冲的附件中, 由于我们创建的帧缓冲不是默认的缓冲, 所以渲染指令不会对当前窗口的输出有任何影响, 故又称为离屏渲染.
    // 创建一个附件的时候, 一般有两个选项: 纹理附件和渲染缓冲对象.
    // (1) 当把一个纹理附加到帧缓冲的时候, 所有的渲染指令将会写入到这个纹理中, 即所有的渲染操作的结果都会存储到一个纹理图像中;
    // (2) 渲染缓冲对象附加的好处是，它会将所有的渲染数据储存在它的缓冲中, 并且格式为OpenGL原生的渲染格式, 所以交换缓冲这样的操作在使用渲染缓冲对象时会非常快. (通常只写)
    // 总结: 渲染缓冲对象能为你的帧缓冲对象提供一些优化，但知道什么时候使用渲染缓冲对象，什么时候使用纹理是很重要的。通常的规则是，如果你不需要从一个缓冲中采样数据，那么对这个缓冲使用渲染缓冲对象会是明智的选择。如果你需要从缓冲中采样颜色或深度值等数据，那么你应该选择纹理附件。性能方面它不会产生非常大的影。


    // (1) 创建纹理附件
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr); // NULL表示仅仅分配内存却没有填充数据
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 将纹理附件附加到帧缓冲上
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);


    // (2) 创建渲染缓冲对象附件
    GLuint RBOID;
    glGenRenderbuffers(1, &RBOID);
    glBindRenderbuffer(GL_RENDERBUFFER, RBOID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // 创建一个深度和模板渲染缓冲对象为了进行深度测试
    glBindRenderbuffer(GL_RED_INTEGER, 0); // 分配内存之后便可以解绑
    // 将渲染缓冲对象附件附加到帧缓冲上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOID);
    
    
    // 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        printf("执行胜利的舞蹈");
    } else {
        printf("当前创建的帧缓冲不完整");
        abort();
    }
    
    glDeleteFramebuffers(1, &FBOID); // 完成所有操作后删除帧缓冲对象
    
}
