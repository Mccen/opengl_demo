#version 330
// 材质属性结构体
struct Material {
    vec4 ambient; // 材质的环境光颜色
    vec3 diffuse; // 材质的漫反射颜色
    vec3 specular; // 材质的镜面反射颜色
    float shininess; // 材质的光泽度
};

// 光源属性结构体
struct Light {
    vec4 ambient; // 光源的环境光颜色
    vec3 diffuse; // 光源的漫反射颜色
    vec3 specular; // 光源的镜面反射颜色
    vec3 position; // 光源的位置
    vec3 direction; // 光源的方向
};

in vec2 fTexCoord; // 传递自顶点着色器的纹理坐标
in vec3 viewLightPos; // 视图光源位置
in vec3 viewNormal;//视觉方向法线
in vec3 varyingLightDir;//指向光照方向
in vec3 viewVerPos;//视觉顶点位置
out vec4 color; // 输出颜色

uniform Light light; // 光源数据
uniform Material material; // 材质数据
uniform sampler2D fTex; // 纹理采样器
void main() {
    // 计算并标准化视图法线向量
    vec3 N = normalize(viewNormal);
    // 计算并标准化光照方向向量
    vec3 L = normalize(varyingLightDir);
    // 计算并标准化观察者方向向量
    vec3 V = normalize(-viewVerPos);
    // 计算光照方向与法线之间夹角的余弦值，并确保其非负
    float cosLN = max(dot(L, N), 0.0);


    // // 计算光照在表面的反射向量
    // vec3 R = reflect(-L, N);
    // 计算反射光方向与观察者方向之间夹角的余弦值，并确保其非负
    // float cosVR = max(dot(V, R), 0.0);
    // 计算最终的镜面反射光强度
    // vec3 specular = light.specular * material.specular* pow(cosVR, material.shininess);

    //半程反射
    vec3 H  = normalize(L + V);
    float cosHN = max(dot(N, H), 0.0);
    // 计算最终的镜面反射光强度
    vec3 specular = light.specular * material.specular * pow(cosHN, material.shininess*2.0)*texture(fTex, fTexCoord).rgb;


    // 计算光源的环境光强度
    vec3 ambientL = light.ambient.rgb * light.ambient.a;
    // 计算材质的环境光强度
    vec3 ambientM = material.ambient.rgb * material.ambient.a;
    // 计算最终的环境光强度
    vec3 ambient = ambientL*ambientM * texture(fTex, fTexCoord).rgb;


    // 计算最终的漫反射光强度
    vec3 diffuse = light.diffuse * material.diffuse * cosLN * texture(fTex, fTexCoord).rgb;



    // 计算最终颜色，将环境光、漫反射光和镜面反射光相加
    color = vec4(ambient + diffuse + specular, 1.0);
}