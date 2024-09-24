#include "noise.hpp"
#include <random>
#include <thread>
// 梯度向量取值区
float radian[36] = {1.00f, 0.984f, 0.939f, 0.866f, 0.766f, 0.642f,
                    0.500f, 0.342f, 0.173f, 0.000f, -0.173f, -0.342f,
                    -0.500f, -0.642f, -0.766f, -0.866f, -0.939f, -0.984f,
                    -1.000f, -0.984f, -0.939f, -0.866f, -0.766f, -0.642f,
                    -0.500f, -0.342f, -0.173f, 0.000f, 0.173f, 0.342f,
                    0.500f, 0.642f, 0.766f, 0.866f, 0.939f, 0.984f};

Noise::Noise(glm::vec2 chunkPosition)
{
    this->chunkPosition = chunkPosition;
    GradientVec();
    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            glm::ivec2* distanceVec = new glm::ivec2[4];
            DistanceVec(glm::ivec2(x,y), distanceVec);
            GLfloat dotNum[4];
            for (int i = 0; i < 4; i++)
            {
                dotNum[i] = dot(glm::vec2(gradientVec[i].x, gradientVec[i].y), glm::vec2(distanceVec[i].x, distanceVec[i].y));
            }
            GLdouble tY = static_cast<double>(y) / 15.0;
            GLdouble tX = static_cast<double>(x) / 15.0;
            GLdouble lerpNumYL = lerp(dotNum[0], dotNum[1], tY);
            GLdouble lerpNumYR = lerp(dotNum[3], dotNum[2], tY);
            GLdouble lerpNumXL = lerp(lerpNumYL, lerpNumYR, tX);
            this->finNum[x][y] = floor(smoothstep(lerpNumXL));
            std::cout << this->finNum[x][y] << " ";
        }
        std::cout << std::endl;
    }
}
Noise::~Noise()
{
}
// 生成随机梯度向量
void Noise::GradientVec()
{
    glm::ivec2 chunkLeftBottom, chunkLeftTop, chunkRightTop, chunkRightBottom;
    GLint chunkX = chunkPosition.x;
    GLint chunkZ = chunkPosition.y;
    chunkLeftBottom = glm::ivec2(chunkX * 16, chunkZ * 16);
    chunkLeftTop = glm::ivec2(chunkX * 16, chunkZ * 16 + 16);
    chunkRightTop = glm::ivec2(chunkX * 16 + 16, chunkZ * 16 + 16);
    chunkRightBottom = glm::ivec2(chunkX * 16 + 16, chunkZ * 16);

    std::mt19937 genLBX(chunkLeftBottom.x);
    std::mt19937 genLBZ(chunkLeftBottom.y);
    std::mt19937 genLTX(chunkLeftTop.x);
    std::mt19937 genLTZ(chunkLeftTop.y);
    std::mt19937 genRTX(chunkRightTop.x);
    std::mt19937 genRTZ(chunkRightTop.y);
    std::mt19937 genRBX(chunkRightBottom.x);
    std::mt19937 genRBZ(chunkRightBottom.y);
    std::uniform_int_distribution<int> dis(0, 35);

    gradientVec[0] = glm::ivec2(radian[dis(genLBX)], radian[dis(genLBZ)]);
    gradientVec[1] = glm::ivec2(radian[dis(genLTX)], radian[dis(genLTZ)]);
    gradientVec[2] = glm::ivec2(radian[dis(genRTX)], radian[dis(genRTZ)]);
    gradientVec[3] = glm::ivec2(radian[dis(genRBX)], radian[dis(genRBZ)]);
}

// 计算方向向量
void Noise::DistanceVec(const glm::ivec2& blockPosition, glm::ivec2* distanceVec)
{
    glm::ivec2 leftBottom(0.0f, 0.0f), leftTop(0.0f, 15.0f), rightTop(15.0f, 15.0f),
        rightBottom(15.0f, 0.0f);
    distanceVec[0] = glm::vec2(leftBottom.x - blockPosition.x, leftBottom.y - blockPosition.y);
    distanceVec[1] = glm::vec2(leftTop.x - blockPosition.x, leftTop.y - blockPosition.y);
    distanceVec[2] = glm::vec2(rightTop.x - blockPosition.x, rightTop.y - blockPosition.y);
    distanceVec[3] = glm::vec2(rightBottom.x - blockPosition.x, rightBottom.y - blockPosition.y);
}

// 计算点积
float Noise::dot(const glm::vec2& a, const glm::vec2& b)
{
    return a.x * b.x + a.y * b.y;
}
// 线性插值（加权平均）
double Noise::lerp(double a, double b, double t) { return a + t * (b - a); }

// 平滑函数
double Noise::smoothstep(double t) { return t * t * (3 - 2 * t); }
