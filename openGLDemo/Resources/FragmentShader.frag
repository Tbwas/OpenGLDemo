#version 330 core

in vec3 inPosition; // 从顶点着色器传来的顶点坐标（类型和名称要相同）
in vec2 inTexCoord;  // 从顶点着色器传来的纹理坐标（类型和名称要相同）
in vec3 inNormalVec; // 从顶点着色器传来的法向量（类型和名称要相同）
in vec3 inFragPos; // 从顶点传来的片段位置（类型和名称要相同）
out vec4 outColor; // 片段着色器输出的变量名可以为任意，但是类型必须为vec4

// 定义一个物体的材质属性，各分量代表物体在不同光照下的颜色，同时应该给每个分量指定一个强度值，不然会显着光照很强、亮度很大，事实上，环境光照对物体颜色的影响最小。
struct Material {
    sampler2D diffuse; // 漫反射贴图
    sampler2D specular; // 镜面光照贴图
    sampler2D emission; // 发光贴图
    float shininess; // 反光度
    
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

// 定向光（平行光）eg: 太阳光
struct DirLight {
    vec3 direction; // 定向光不需要位置
    
    vec3 ambient; // 环境光照通常会设置为一个比较低的强度，因为我们不希望环境光颜色太过显眼
    vec3 diffuse; // 光源的漫反射分量通常设置为光所具有的颜色，通常是一个比较明亮的白色
    vec3 specular; // 镜面光分量通常会保持为vec3(1.0)，以最大强度发光
};
uniform DirLight dirLight;

// 点光源 eg: 灯泡
struct PointLight {
    vec3 position;
    
    vec3 ambient; // 环境光照通常会设置为一个比较低的强度，因为我们不希望环境光颜色太过显眼
    vec3 diffuse; // 光源的漫反射分量通常设置为光所具有的颜色，通常是一个比较明亮的白色
    vec3 specular; // 镜面光分量通常会保持为vec3(1.0)，以最大强度发光
    
    float constant; // 计算衰减的常数项
    float linear; // 计算衰减的一次项系数
    float quadratic; // 计算衰减的二次项系数
};
#define NR_POINT_LIGHTS 1 // 点光源的数量
uniform PointLight pointLight[NR_POINT_LIGHTS];

// uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，全局的
uniform float textureAlpha;
uniform vec3 viewPosition; // 观察者的世界坐标
uniform vec3 objectColor; // 物体的颜色
uniform Material material; // 物体的材质

/**********************************函数声明******************************************************/

// 定向光的计算
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);

// 点光源的计算
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


/**********************************Main函数******************************************************/


void main() {
    // 属性
    vec3 viewDir = viewPosition - inFragPos;
    
    // 第一阶段：定向光
    vec3 result = CalculateDirLight(dirLight, inNormalVec, viewDir);
    
    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalculatePointLight(pointLight[i], inNormalVec, inFragPos, viewDir);
    }
    
    // outColor = vec4(result, 1.0);
    result = vec3(texture(material.texture_diffuse1, inTexCoord));
    outColor = vec4(result, 1.0);
}

/**********************************函数实现******************************************************/

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    normal = normalize(normal); // 确保法向量（垂直于顶点表面）为单位向量
    viewDir = normalize(viewDir); // 确保视线向量（由片段指向眼睛）为单位向量
    
    vec3 lightDir = normalize(-light.direction); // 由片段指向定向光源的方向向量
    
    float diffEffect = max(dot(normal, lightDir), 0.0f); // 漫反射影响因子
    vec3 reflectDir = reflect(-lightDir, normal); // 第一个向量要求是从光源指向片段方向的，所以对`lightDir`取反
    float specEffect = max(dot(reflectDir, viewDir), 0.0f); // 镜面光影响因子
    specEffect = pow(specEffect, material.shininess); // 高光的反光度
    
    // 计算光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, inTexCoord));
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, inTexCoord)) * diffEffect;
    vec3 specular = light.specular * vec3(texture(material.specular, inTexCoord)) * specEffect;
    
    return (ambient + diffuse + specular);
}


vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    normal = normalize(normal);
    viewDir = normalize(viewDir);
    
    // 点光源的衰减值
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance); // 将计算后的衰减值乘以光照各个分量
    
    vec3 lightDir = normalize(vec3(light.position - fragPos)); // @Note 一定要对向量进行标准化.
    
    float diffEffect = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specEffect = max(dot(reflectDir, viewDir), 0.0f);
    specEffect = pow(specEffect, material.shininess); // 高光的反光度
    
    // 计算光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, inTexCoord)) * attenuation;
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, inTexCoord)) * diffEffect * attenuation;
    vec3 specular = light.specular * vec3(texture(material.specular, inTexCoord)) * specEffect * attenuation;
    
    return (ambient + diffuse + specular);
}

/** 发射光计算
 vec3 CaculateEmissionLight () {
 light.ambient * vec3(texture(material.emission, inTexCoord));
 return emission;
 }
 */


















// ----- 分割线 ---- //
