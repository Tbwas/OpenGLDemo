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
    GLuint textureID; // 用于外部从该纹理中读取像素数据
    GLuint RBOID; // 用于外部从renderBuffer中读取深度值
    
    FrameBuffer (){
        printf("FrameBuffer initialized.");
    }
    ~FrameBuffer(){
        printf("FrameBuffer dealloced.");
        destory();
    }
    
    void GenerateFBO(unsigned int width, unsigned height);
    void bindFBO(GLuint width, GLuint height);
    void unBindFBO(GLuint width, GLuint height);
    
private:
    GLuint FBOID;
    
    void destory() {
        glDeleteFramebuffers(1, &FBOID);
        glDeleteTextures(1, &textureID);
    }
};

#endif 
