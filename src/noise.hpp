#include "headerLists.hpp"
#include <cmath>

struct Noise//柏林噪声
{
    //当前区块的位置
    glm::vec2 chunkPosition;
    //当前区块分配到的梯度向量
    glm::ivec2 gradientVec[4];
    GLint finNum[16][16];
    Noise(glm::vec2 chunkPosition);
    ~Noise();
    // 生成随机梯度向量
    void GradientVec();
    // 计算方向向量
    void DistanceVec(const glm::ivec2& blockPosition, glm::ivec2* distanceVec);
    // 计算点积
    float dot(const glm::vec2& a, const glm::vec2& b);
    // 线性插值（加权平均）
    double lerp(double a, double b, double t);
    // 平滑函数
    double smoothstep(double t);
};
