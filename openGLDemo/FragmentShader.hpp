//
//  FragmentShader.hpp
//  openGLDemo
//
//  Created by xindong on 2018/3/5.
//  Copyright © 2018年 xindong. All rights reserved.
//

#ifndef FragmentShader_hpp
#define FragmentShader_hpp
/**
 OpenGL中的一个片段是OpenGL渲染一个像素所需的所有数据。
 片段着色器的主要目的是计算一个像素的最终颜色，这也是所有OpenGL高级效果产生的地方。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素lStencil的颜色。
 */
#include <stdio.h>
#include <iostream>

class FragmentShader {
    
public:
    uint32_t createFragmentShader(std::string path);
};

#endif
