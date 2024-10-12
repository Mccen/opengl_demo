#ifndef NOISE_HPP
#define NOISE_HPP
#include "../headerLists.hpp"

struct Noise // 柏林噪声
{
    // 当前区块的位置
    glm::vec2 chunkPosition;

    // 当前区块分配到的梯度向量
    glm::vec2 gradientVec[4];

    // 结果
    GLfloat finNum[16][16];
    // 传入方块位置，区块位置
    Noise(const glm::vec2 chunkPosition);
    Noise() = default;
    ~Noise() = default;
    // 生成随机梯度向量
    void GradientVec();
    // 计算方向向量
    void DistanceVec(const glm::vec2 &blockPosition, glm::vec2 (&distanceVec)[4]);
    // 计算点积
    float dot(const glm::vec2 &a, const glm::vec2 &b);
    // 线性插值（加权平均）
    double lerp(float a, float b, float t);
    // 平滑函数
    float smoothstep(float t);
};

#endif // NOISE_HPP
