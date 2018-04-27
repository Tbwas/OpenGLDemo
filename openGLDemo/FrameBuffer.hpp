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
    FrameBuffer();
};

#endif /* FrameBuffer_hpp */
