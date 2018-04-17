# version 330 core

out vec4 outColor;

// 定义一个光结构体
struct Light {
    vec3 position;
    vec3 ambient; // 环境光照通常会设置为一个比较低的强度，因为我们不希望环境光颜色太过显眼。
    vec3 diffuse; // 光源的漫反射分量通常设置为光所具有的颜色，通常是一个比较明亮的白色。
    vec3 specular; // 镜面光分量通常会保持为vec3(1.0)，以最大强度发光。
};

uniform Light light;
uniform vec3 lightColor;

void main () {
    outColor = vec4(lightColor, 1.0);
}
