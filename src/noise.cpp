#include "noise.hpp"
#include <random>
#include <thread>
// 梯度向量取值区
glm::vec2 radian[8] = {
    {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
// glm::vec2 distanceVec[16][16][4]={

// }
Noise::Noise(const glm::vec2 blockPosition, const glm::vec2 chunkPosition)
{
    this->chunkPosition = chunkPosition;
    this->GradientVec();
    glm::ivec2 distanceVec[4];
    this->DistanceVec(blockPosition, distanceVec);
    float dotFloat[4];
    for (int i = 0; i < 4; i++)
    {
        dotFloat[i] = this->dot(distanceVec[i], this->gradientVec[i]);
    }
    float u = fade(blockPosition.x);
    float v = fade(blockPosition.y);
    float tempu1, tempu2, tempv;
    tempu1 = this->lerp(dotFloat[0], dotFloat[1], u);
    tempu2 = this->lerp(dotFloat[3], dotFloat[2], u);
    this->finNum = this->lerp(tempu1, tempu2, v);
}
Noise::~Noise()
{
}
// 哈希函数
float hash(int x, int y)
{
    return static_cast<float>((x + y) / (x == 0 ? 1 : x));
}
// 生成随机梯度向量
void Noise::GradientVec()
{
    float hash1 = hash(this->chunkPosition.x, this->chunkPosition.y);
    float hash2 = hash(this->chunkPosition.x, this->chunkPosition.y + 1);
    float hash3 = hash(this->chunkPosition.x + 1, this->chunkPosition.y + 1);
    float hash4 = hash(this->chunkPosition.x + 1, this->chunkPosition.y);
    std::mt19937_64 genLB(hash1);
    std::mt19937_64 genLT(hash2);
    std::mt19937_64 genRT(hash3);
    std::mt19937_64 genRB(hash4);
    std::uniform_real_distribution<> dis(0, 7);
    this->gradientVec[0] = radian[static_cast<int>(dis(genLB))];
    this->gradientVec[1] = radian[static_cast<int>(dis(genLT))];
    this->gradientVec[2] = radian[static_cast<int>(dis(genRT))];
    this->gradientVec[3] = radian[static_cast<int>(dis(genRB))];
}

// 计算方向向量
void Noise::DistanceVec(const glm::ivec2 &blockPosition, glm::ivec2 (&distanceVec)[4])
{
    glm::ivec2 leftBottom(0, 0);
    glm::ivec2 temp;
    temp.x = leftBottom.x - blockPosition.x;
    temp.y = leftBottom.y - blockPosition.y;
    distanceVec[0] = glm::vec2(temp.x, temp.y);
    distanceVec[1] = glm::vec2(temp.x, temp.y + 16);
    distanceVec[2] = glm::vec2(temp.x + 16, temp.y + 16);
    distanceVec[3] = glm::vec2(temp.x + 16, temp.y);
}

// 计算点积
float Noise::dot(const glm::vec2 &a, const glm::vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}
// 线性插值（加权平均）
double Noise::lerp(float a, float b, float t) { return a + t * (b - a); }

// 平滑插值函数
float Noise::fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}