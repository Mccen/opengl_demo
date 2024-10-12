#include "Noise.hpp"
#include <random>
Noise::Noise(const glm::vec2 chunkPosition)
{
    this->chunkPosition = chunkPosition;
    this->GradientVec();
    for (int i = 0; i < 16; i++)
    {
        for (int ii = 0; ii < 16; ii++)
        {
            glm::vec2 distanceVec[4];
            this->DistanceVec(glm::vec2(i, ii), distanceVec);
            float dotFloat[4];
            for (int iii = 0; iii < 4; iii++)
            {
                dotFloat[iii] = this->dot(distanceVec[iii], this->gradientVec[iii]);
            }
            float u = static_cast<float>(i + 1) / 16;
            float v = static_cast<float>(ii + 1) / 16;
            float tempu1, tempu2, tempv;
            tempu1 = this->lerp(dotFloat[0], dotFloat[1], u);
            tempu2 = this->lerp(dotFloat[3], dotFloat[2], u);
            tempv = this->lerp(tempu1, tempu2, v);
            this->finNum[i][ii] = static_cast<float>(((smoothstep(tempv)/2)>=0.0f)?(smoothstep(tempv)/2):0);
        }
    }
}
// 哈希函数
float hash(int x, int y)
{
    size_t seed = static_cast<size_t>(x) ^ (static_cast<size_t>(y) << 1);
    return static_cast<float>(seed % 1000) / 1000.0f; // 归一化到 [0, 1]
}
// 生成随机梯度向量
void Noise::GradientVec()
{
    float hash1 = hash(this->chunkPosition.x, this->chunkPosition.y);
    float hash2 = hash(this->chunkPosition.x, this->chunkPosition.y + 1);
    float hash3 = hash(this->chunkPosition.x + 1, this->chunkPosition.y + 1);
    float hash4 = hash(this->chunkPosition.x + 1, this->chunkPosition.y);
    std::mt19937 genLB(static_cast<unsigned long long>(hash1 * 1000000));
    std::mt19937 genLT(static_cast<unsigned long long>(hash2 * 1000000));
    std::mt19937 genRT(static_cast<unsigned long long>(hash3 * 1000000));
    std::mt19937 genRB(static_cast<unsigned long long>(hash4 * 1000000));
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f); // 生成 [-1, 1] 区间的随机数
                                                            // 生成四个随机梯度向量
    this->gradientVec[0] = glm::vec2(std::make_pair(dis(genLB), dis(genLB)).first, std::make_pair(dis(genLB), dis(genLB)).second);
    this->gradientVec[1] = glm::vec2(std::make_pair(dis(genLT), dis(genLT)).first, std::make_pair(dis(genLT), dis(genLT)).second);
    this->gradientVec[2] = glm::vec2(std::make_pair(dis(genRT), dis(genRT)).first, std::make_pair(dis(genRT), dis(genRT)).second);
    this->gradientVec[3] = glm::vec2(std::make_pair(dis(genRB), dis(genRB)).first, std::make_pair(dis(genRB), dis(genRB)).second);
}

// 计算方向向量
void Noise::DistanceVec(const glm::vec2 &blockPosition, glm::vec2 (&distanceVec)[4])
{
    glm::ivec2 leftBottom(0, 0);
    glm::ivec2 temp;
    temp.x = leftBottom.x - blockPosition.x;
    temp.y = leftBottom.y - blockPosition.y;
    distanceVec[0] = glm::vec2(static_cast<float>(temp.x) / 16, static_cast<float>(temp.y) / 16);
    distanceVec[1] = glm::vec2(static_cast<float>(temp.x) / 16, static_cast<float>(temp.y + 16) / 16);
    distanceVec[2] = glm::vec2(static_cast<float>(temp.x + 16) / 16, static_cast<float>(temp.y + 16) / 16);
    distanceVec[3] = glm::vec2(static_cast<float>(temp.x + 16) / 16, static_cast<float>(temp.y) / 16);
}

// 计算点积
float Noise::dot(const glm::vec2 &a, const glm::vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}
// 线性插值（加权平均）
double Noise::lerp(float a, float b, float t) { return a + t * (b - a); }

// 平滑插值函数
float Noise::smoothstep(float t)
{
    return t * t * (3 - 2 * t);
}