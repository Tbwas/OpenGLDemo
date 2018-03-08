//
//  DataSource.hpp
//  openGLDemo
//
//  Created by xindong on 2018/3/8.
//  Copyright © 2018年 xindong. All rights reserved.
//

#ifndef DataSource_hpp
#define DataSource_hpp

#include <stdio.h>

/**
 告诉OpenGL如何解析顶点数据
 
 @param 0 在顶点着色器源码中，layout(location = 0) 定义了position顶点属性位置，把数据传递到该顶点属性中
 @param 3 顶点属性分量数量（position包含x、y、z共3个分量）
 @param GL_FLOAT 数据类型
 @param GL_FALSE 是否希望数据被标准化
 @param GLfloat 步长，即连续的顶点之间的间隔（或者设为0自动判断）
 @param GLvoid* 表示顶点属性在缓冲中起始位置的偏移量
 */

extern void glVertexAttribPointer_(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);


/**
 用图片数据生成纹理
 
 @param target 纹理对象所绑定的纹理目标
 @param level 多级渐远纹理的级别，0表示基本级别
 @param internalformat 纹理存储为何种格式，图片只有RGB格式
 @param width 最终纹理的宽度
 @param height 最终纹理的高度
 @param border 总是为0，历史遗留问题
 @param format 源图的格式
 @param type 源图的数据类型
 @param pixels 源图数据
 */
extern void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);

#pragma mark -

class DataSource {
    
public:
    GLuint* setupData(); // 返回VAOs
};

#endif
