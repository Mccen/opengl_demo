#ifndef NOISE_HPP
#define NOISE_HPP
#include "headerLists.hpp"
#include <cmath>

struct Noise // 柏林噪声
{
    // 当前区块的位置
    glm::vec2 chunkPosition;
    // 当前区块分配到的梯度向量
    glm::vec2 gradientVec[4];
    //结果
    GLfloat finNum;
    // 传入方块位置，区块位置
    Noise(){}
    Noise(const glm::vec2 blockPosition, const glm::vec2 chunkPosition);
    ~Noise();
    // 生成随机梯度向量
    void GradientVec();
    // 计算方向向量
    void DistanceVec(const glm::ivec2 &blockPosition, glm::ivec2 (&distanceVec)[4]);
    // 计算点积
    float dot(const glm::vec2 &a, const glm::vec2 &b);
    // 线性插值（加权平均）
    double lerp(float a, float b, float t);
    // 平滑函数
    float fade(float t);
};

#endif // NOISE_HPP
