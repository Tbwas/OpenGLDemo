#version 330 core

in vec3 outPosition; // 从顶点着色器传来的顶点坐标（类型和名称要相同）
in vec2 outTexture;  // 从顶点着色器传来的纹理坐标（类型和名称要相同）
in vec3 outNormalVec; // 从顶点着色器传来的法向量（类型和名称要相同）
in vec3 outFragPosition; // 从顶点传来的片段位置（类型和名称要相同）

out vec4 color; // 片段着色器输出的变量名可以为任意，但是类型必须为vec4

// 定义一个物体的材质属性，各分量代表物体在不同光照下的颜色，同时应该给每个分量指定一个强度值，不然会显着光照很强、亮度很大，事实上，环境光照对物体颜色的影响最小。
struct Material {
    vec3 ambient; // 该向量定义了在环境光照下这个物体反射得是什么颜色，通常这是和物体颜色相同的颜色。
    vec3 diffuse; // 该向量定义了在漫反射光照下物体的颜色。
    vec3 specular; // 该向量设置的是镜面光照对物体的颜色影响。
    float shininess; // 反光度
};

// 定义一个光结构体
struct Light {
    vec3 position;
    vec3 ambient; // 环境光照通常会设置为一个比较低的强度，因为我们不希望环境光颜色太过显眼。
    vec3 diffuse; // 光源的漫反射分量通常设置为光所具有的颜色，通常是一个比较明亮的白色。
    vec3 specular; // 镜面光分量通常会保持为vec3(1.0)，以最大强度发光。
};

// uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，全局的
uniform sampler2D ourTexture1; // 通过采样器来获取纹理对象(即贴图)
uniform sampler2D ourTexture2; // 再定义一个采样器来获取另一个纹理
uniform float textureAlpha;

uniform vec3 viewPosition; // 观察者的世界坐标
uniform vec3 objectColor; // 物体的颜色

uniform Material material; // 物体的材质
uniform Light light; // 光

void main() {
    
    // 环境光照
    vec3 ambient = material.ambient * light.ambient; // 使用环境光照很简单，用光颜色，乘以物体的颜色。
    
    // 漫反射光照
    vec3 norm = normalize(outNormalVec); // 法线向量标准化，转为单位向量
    vec3 lightDir = normalize(light.position - outFragPosition); // 计算光源和片段位置之间的方向向量（即由片段位置指向光源位置）
    float affect = dot(norm, lightDir); // 两个单位向量点乘得到两者夹角的余弦值，作为光源对当前片段漫发射的影响因子
    affect = max(affect, 0);
    vec3 diffuse = affect * material.diffuse * light.diffuse; // 影响因子和光的颜色相乘，得到漫反射分量，夹角越大，漫反射分量就越小
    
    // 镜面光照
    vec3 viewDir = normalize(viewPosition - outFragPosition); // 计算出视线方向向量
    vec3 reflectDir = reflect(-lightDir, outNormalVec); // 通过入射向量和法向量计算出反射向量
    float cosVar = max(dot(viewDir, reflectDir), 0); // 视线向量和法线向量夹角的余弦值
    float spec = pow(cosVar, material.shininess); // 32是高光的反光度，一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
    vec3 specular = material.specular * spec * light.specular; // 计算出镜面分量
    
    // 把环境分量、漫反射分量、镜面分量相加，所得结果再乘以物体的颜色，即为片段最终输出的颜色
    vec3 result = (ambient + diffuse + specular);
    color = vec4(result, 1.0);
    
    // color = vec4(outColor, 1.0);
    // color = vec4(outPosition, 1.0);
    // color = vec4(0.0, 1.0, 0.0, 1.0);
    // color = texture(ourTexture1, outTexture) * vec4(outColor, 1.0);
    // color = mix(texture(ourTexture1, outTexture), texture(ourTexture2, outTexture), textureAlpha) * vec4(outColor, 1.0); // 0.3表示返回第一个颜色70%和第二个颜色30%的混合色
    // color = texture(ourTexture1, outTexture, textureAlpha) * vec4(outColor, 1.0);
    // color = mix(texture(ourTexture1, outTexture), texture(ourTexture1, outTexture), 0.5);
}

