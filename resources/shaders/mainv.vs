#version 330

// 定义材质属性结构体
struct Material {
    vec4 ambient; // 材质的环境光颜色
    vec3 diffuse; // 材质的漫反射颜色
    vec3 specular; // 材质的镜面反射颜色
    float shininess; // 材质的光泽度
};

// 定义光源属性结构体
struct Light {
    vec4 ambient; // 光源的环境光颜色
    vec3 diffuse; // 光源的漫反射颜色
    vec3 specular; // 光源的镜面反射颜色
    vec3 position; // 光源的位置
    vec3 direction; // 光源的方向

};

layout (location = 0) in vec3 vPos; // 顶点位置
layout (location = 1) in vec3 vNormal; // 顶点法线
layout (location = 2) in vec2 vTexCoord; // 顶点纹理坐标

out vec2 fTexCoord; // 传递到片段着色器的顶点纹理坐标
out vec3 viewLightPos; // 视图光源位置
out vec3 viewNormal;//视觉方向法线
out vec3 varyingLightDir;//指向光照方向
out vec3 viewVerPos;//视觉顶点位置
uniform mat4 lmodel; // 灯光模型矩阵
uniform mat4 model; // 模型矩阵
uniform mat4 view; // 视图矩阵
uniform mat4 projection; // 投影矩阵
uniform Light light; // 光源数据
uniform Material material; // 材质数据

void main() {
    // 计算最终的顶点位置
    gl_Position = projection * view * model*vec4(normalize(vPos), 1.0);
    // 计算顶点在视图空间中的位置
    viewVerPos = vec3(view * model*vec4(vPos, 1.0));
    // 计算光源在视图空间中的位置
    viewLightPos = vec3(view * lmodel * vec4(light.position, 1.0));
    // 计算顶点在视图空间中的法线方向
    viewNormal = vec3(transpose(inverse(view*model)) * vec4(vNormal,1.0f));
    // 计算从顶点到光源的方向
    varyingLightDir =  viewLightPos - viewVerPos;

    // 传递纹理坐标
    fTexCoord = vTexCoord;
}