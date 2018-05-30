//
//  FrameBuffer.hpp
//  openGLDemo
//
//  Created by xindong on 2018/4/27.
//  Copyright © 2018年 xindong. All rights reserved.
//

#ifndef FrameBuffer_hpp
#define FrameBuffer_hpp

#include <OpenGL/gl3.h>
#include <stdio.h>

/**一个完整的帧缓冲必须满足以下四个条件：
     1. 附加至少一个缓冲（颜色、深度或模板缓冲）;
     2. 至少有一个颜色附件(Attachment);
     3. 所有的附件都必须是完整的（保留了内存）;
     4. 每个缓冲都应该有相同的样本数.
 */
class FrameBuffer {
    
public:
    GLuint FBOID;
    GLuint textureID; // 用于外部从该纹理中读取像素数据
    
    FrameBuffer (){
        printf("FrameBuffer initialized.");
    }
    ~FrameBuffer(){
        printf("FrameBuffer dealloced.");
        destory();
    }
    
    /// Keep in mind that when you resize your window, you have to resize these textures too and call this method.
    void GenerateFBO(unsigned int width, unsigned height);
    
private:
    void destory() {
        glDeleteFramebuffers(1, &FBOID);
        glDeleteTextures(1, &textureID);
    }
};

#endif /* FrameBuffer_hpp */
