#include <unordered_set>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <GL/glew.h>
//gpt4o二次优化
enum type : unsigned int
{
    stone = 0,
    grass,
    air,
    dirt,
    sand,
    snow,
    water,
    lava,
    leaves,
    planks,
    sapling,
    cobblestone,
    gravel,
    gold,
    iron,
    diamond,
    redstone,
    lapis,
    emerald,
    obsidian,
    netherrack,
};

// 定义单个方块的结构体
struct block
{
    glm::vec3 b_position; // 方块位置
    type b_type;          // 方块类型
    GLuint b_tex;         // 纹理ID
    block(glm::vec3 position, type type, GLuint tex)
        : b_position(position), b_type(type), b_tex(tex) {}
};

// 定义区块的结构体
struct chunk
{
    glm::vec2 c_position; // 区块位置
    bool c_isGenerated = false; // 是否生成标志
    int c_size = 16;      // 区块大小
    std::vector<block> c_blocks; // 区块包含的方块列表

    chunk() {}
    chunk(glm::vec2 position) : c_position(position) {}

    // 生成方块（在区块内填充stone方块）
    void createBlock()
    {
        for (int x = this->c_position.x * c_size; x < (this->c_position.x + 1) * c_size; x++)
        {
            for (int z = this->c_position.y * c_size; z < (this->c_position.y + 1) * c_size; z++)
            {
                this->c_blocks.emplace_back(glm::vec3(x, 0, z), stone, 0);
            }
        }
        this->c_isGenerated = true;
    }
};

// 区块的哈希函数，用于unordered_set存储
struct chunkHash
{
    std::size_t operator()(const chunk &c) const
    {
        std::size_t hx = std::hash<int>()(c.c_position.x);
        std::size_t hy = std::hash<int>()(c.c_position.y);
        return hx ^ (hy << 1); // 使用位移减少哈希冲突
    }
};

// 区块的相等比较函数，用于unordered_set存储
struct chunkEqual
{
    bool operator()(const chunk &c1, const chunk &c2) const
    {
        return c1.c_position == c2.c_position;
    }
};

chunk viewChunk;
int chunkNum = 1;

std::unordered_set<chunk, chunkHash, chunkEqual> chunkHas;
std::vector<chunk> chunks;

// 计算视角所在的区块
void getViewChunk(glm::vec3 viewPosition)
{
    glm::vec2 chunkPosition;
    chunkPosition.x = viewPosition.x / 16.0f;
    chunkPosition.y = viewPosition.z / 16.0f;
    viewChunk.c_position = chunkPosition;
    viewChunk.c_isGenerated = true;
}

// 检查区块是否已经生成
bool hasChunk(const chunk &temp)
{
    return chunkHas.find(temp) != chunkHas.end();
}

// 加载并生成当前视野范围内的区块
void loadChunk()
{
    chunks.clear();
    for (int i = viewChunk.c_position.x - chunkNum; i < viewChunk.c_position.x + chunkNum; i++)
    {
        for (int ii = viewChunk.c_position.y - chunkNum; ii < viewChunk.c_position.y + chunkNum; ii++)
        {
            chunk tempChunk(glm::vec2(i, ii));

            if (!hasChunk(tempChunk))
            {
                tempChunk.createBlock();
                chunkHas.insert(tempChunk);
            }
            chunks.push_back(tempChunk);
        }
    }
}

// 生成单个方块的顶点数据
std::vector<float> generateBlockVertices(const block &b)
{
    constexpr int VERTICES_PER_FACE = 6; // 每个面6个顶点（三角形）
    constexpr int ATTRIBUTES_PER_VERTEX = 8; // 每个顶点8个属性（位置+纹理+法线）
    constexpr int FACES_PER_BLOCK = 6; // 每个方块6个面
    std::vector<float> vertices(VERTICES_PER_FACE * FACES_PER_BLOCK * ATTRIBUTES_PER_VERTEX);

    glm::vec3 blockPos = b.b_position; // 获取方块位置

    // 各个面法线方向
    glm::vec3 normals[] = {
        glm::vec3(0.0f, -1.0f, 0.0f), // 下
        glm::vec3(0.0f, 1.0f, 0.0f),  // 上
        glm::vec3(0.0f, 0.0f, 1.0f),  // 前
        glm::vec3(0.0f, 0.0f, -1.0f), // 后
        glm::vec3(-1.0f, 0.0f, 0.0f), // 左
        glm::vec3(1.0f, 0.0f, 0.0f)   // 右
    };

    // 纹理坐标
    float textureCoords[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 下
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 上
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 前
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 后
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // 左
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f  // 右
    };

    int vertexIndex = 0;

    // 根据不同面生成顶点
    for (int face = 0; face < FACES_PER_BLOCK; ++face)
    {
        glm::vec3 v0, v1, v2, v3;
        switch (face)
        {
            case 0: // 下
                v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z);
                v1 = glm::vec3(blockPos.x + 1.0f, blockPos.y, blockPos.z);
                v2 = glm::vec3(blockPos.x + 1.0f, blockPos.y, blockPos.z + 1.0f);
                v3 = glm::vec3(blockPos.x, blockPos.y, blockPos.z + 1.0f);
                break;
            case 1: // 上
                v0 = glm::vec3(blockPos.x, blockPos.y + 1.0f, blockPos.z);
                v1 = glm::vec3(blockPos.x + 1.0f, blockPos.y + 1.0f, blockPos.z);
                v2 = glm::vec3(blockPos.x + 1.0f, blockPos.y + 1.0f, blockPos.z + 1.0f);
                v3 = glm::vec3(blockPos.x, blockPos.y + 1.0f, blockPos.z + 1.0f);
                break;
            case 2: // 前
                v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z);
                v1 = glm::vec3(blockPos.x + 1.0f, blockPos.y, blockPos.z);
                v2 = glm::vec3(blockPos.x + 1.0f, blockPos.y + 1.0f, blockPos.z);
                v3 = glm::vec3(blockPos.x, blockPos.y + 1.0f, blockPos.z);
                break;
            case 3: // 后
                v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z + 1.0f);
                v1 = glm::vec3(blockPos.x + 1.0f, blockPos.y, blockPos.z + 1.0f);
                v2 = glm::vec3(blockPos.x + 1.0f, blockPos.y + 1.0f, blockPos.z + 1.0f);
                v3 = glm::vec3(blockPos.x, blockPos.y + 1.0f, blockPos.z + 1.0f);
                break;
            case 4: // 左
                v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z);
                v1 = glm::vec3(blockPos.x, blockPos.y + 1.0f, blockPos.z);
                v2 = glm::vec3(blockPos.x, blockPos.y + 1.0f, blockPos.z + 1.0f);
                v3 = glm::vec3(blockPos.x, blockPos.y, blockPos.z + 1.0f);
                break;
            case 5: // 右
                v0 = glm::vec3(blockPos.x + 1.0f, blockPos.y, blockPos.z);
                v1 = glm::vec3(blockPos.x + 1.0f, blockPos.y + 1.0f, blockPos.z);
                v2 = glm::vec3(blockPos.x + 1.0f, blockPos.y + 1.0f, blockPos.z + 1.0f);
                v3 = glm::vec3(blockPos.x + 1.0f, blockPos.y, blockPos.z + 1.0f);
                break;
        }

        // 插入两个三角形的顶点数据
        for (int i = 0; i < 6; ++i)
        {
            const glm::vec3 &vertexPos = (i < 3) ? (i == 0 ? v0 : i == 1 ? v1 : v2) : (i == 3 ? v0 : v2);
            vertices[vertexIndex++] = vertexPos.x;
            vertices[vertexIndex++] = vertexPos.y;
            vertices[vertexIndex++] = vertexPos.z;
            vertices[vertexIndex++] = textureCoords[face * 8 + i * 2];
            vertices[vertexIndex++] = textureCoords[face * 8 + i * 2 + 1];
            vertices[vertexIndex++] = normals[face].x;
            vertices[vertexIndex++] = normals[face].y;
            vertices[vertexIndex++] = normals[face].z;
        }
    }

    return vertices;
}

// 生成区块的顶点数据
std::vector<float> generateChunkVertices(const chunk &c)
{
    std::vector<float> vertices;
    vertices.reserve(c.c_blocks.size() * 36 * 8); // 预留足够的空间以避免重新分配

    for (const auto &block : c.c_blocks)
    {
        const std::vector<float> &blockVertices = generateBlockVertices(block);
        vertices.insert(vertices.end(), blockVertices.begin(), blockVertices.end());
    }

    return vertices;
}

// 渲染所有区块
void renderChunk()
{
    static bool isInitialized = false;
    static GLuint VAO, VBO;

    if (!isInitialized)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // 将所有区块的顶点数据一次性传递给OpenGL
        std::vector<float> allVertices;
        for (const auto &c : chunks)
        {
            std::vector<float> chunkVertices = generateChunkVertices(c);
            allVertices.insert(allVertices.end(), chunkVertices.begin(), chunkVertices.end());
        }

        glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), &allVertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        isInitialized = true;
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, chunks.size() * 36 * 6); // 假设每个区块都有36个三角形面
}
