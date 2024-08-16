#include <unordered_set>
enum type : unsigned int
{
    stone = 1,
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
struct block
{
    glm::vec3 b_position;
    type b_type;
    block(glm::vec3 position, type type)
    {
        b_position = position;
        b_type = type;
    }
};
struct chunk
{
    // chunk的位置
    glm::vec2 c_position;
    // 是否生成过
    bool c_isGenerated = false;
    // 区块边长
    int c_size = 16;
    std::vector<block> c_blocks;
    chunk() {}
    chunk(glm::vec2 position) : c_position(position)
    {
    }
    void createBlock()
    {
        for (int x = this->c_position.x * c_size; x < (this->c_position.x + 1) * c_size; x++)
        {
            for (int z = this->c_position.y * c_size; z < (this->c_position.y + 1) * c_size; z++)
            {
                this->c_blocks.push_back(block(glm::vec3(x, 0, z), stone));
            }
        }
        this->c_isGenerated = true;
    }
};
struct chunkHash // 哈希函数
{
    std::size_t operator()(const chunk &c) const
    {
        return std::hash<int>()(c.c_position.x) ^ std::hash<int>()(c.c_position.y);
    }
};
struct chunkEqual // 哈希比较器
{
    bool operator()(const chunk &c1, const chunk &c2) const
    {
        return c1.c_position == c2.c_position;
    }
};
chunk viewChunk;
int chunkNum = 2;

// 创建chunk查找结构，留档已创建的区块
std::unordered_set<chunk, chunkHash, chunkEqual> chunkHas;
// 储存的要使用的chunk
std::vector<chunk> chunks;

void getViewChunk(glm::vec3 viewPosition)
{
    glm::vec2 chunkPosition;
    chunkPosition.x = static_cast<int>(viewPosition.x / 16);
    chunkPosition.y = static_cast<int>(viewPosition.z / 16);
    viewChunk.c_position = chunkPosition;
    std::cout << "viewChunk: " << viewChunk.c_position.x << " " << viewChunk.c_position.y << std::endl;
    viewChunk.c_isGenerated = true;
}
bool hasChunk(chunk temp)
{
    if (chunkHas.find(temp) != chunkHas.end())
        return true;
    else
        return false;
}
void loadChunk()
{
    // chunkHas.clear();
    chunks.clear();
    for (int i = viewChunk.c_position.x - chunkNum; i <= viewChunk.c_position.x + chunkNum; i++)
    {
        for (int ii = viewChunk.c_position.y - chunkNum; ii <= viewChunk.c_position.y + chunkNum; ii++)
        {
            chunk tempChunk(glm::vec2(i, ii));

            if (!hasChunk(tempChunk))
            {
                tempChunk.createBlock();
                chunkHas.insert(tempChunk);
                chunks.push_back(tempChunk);
                std::cout<<"new"<<std::endl;
            }
            else
            {
                chunks.push_back(*chunkHas.find(tempChunk));
                //std::cout<<"finded"<<std::endl;
            }
        }
    }
}
// 生成单个方块的顶点数据
std::vector<float> generateBlockVertices(const block &b)
{
    glm::vec3 position = b.b_position;
    const float blockSize = 1.0f;
    glm::vec3 blockPos = position;

    std::vector<float> vertices(36 * 8); // 每个顶点有8个属性：3个位置坐标+2个纹理坐标+3个法线坐标

    glm::vec3 normals[] = {
        glm::vec3(0.0f, -1.0f, 0.0f), // 下
        glm::vec3(0.0f, 1.0f, 0.0f),  // 上
        glm::vec3(0.0f, 0.0f, 1.0f),  // 前
        glm::vec3(0.0f, 0.0f, -1.0f), // 后
        glm::vec3(-1.0f, 0.0f, 0.0f), // 左
        glm::vec3(1.0f, 0.0f, 0.0f)   // 右
    };

    float textureCoords[] = {
        0.0f, 0.0f, // 下
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f, // 上
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f, // 前
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f, // 后
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f, // 左
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f, // 右
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f};

    int vertexIndex = 0;

    // 生成每个面的顶点
    for (int face = 0; face < 6; ++face)
    {
        glm::vec3 v0, v1, v2, v3;
        switch (face)
        {
        case 0: // 下
            v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z);
            v1 = glm::vec3(blockPos.x + blockSize, blockPos.y, blockPos.z);
            v2 = glm::vec3(blockPos.x + blockSize, blockPos.y, blockPos.z + blockSize);
            v3 = glm::vec3(blockPos.x, blockPos.y, blockPos.z + blockSize);
            break;
        case 1: // 上
            v0 = glm::vec3(blockPos.x, blockPos.y + blockSize, blockPos.z);
            v1 = glm::vec3(blockPos.x + blockSize, blockPos.y + blockSize, blockPos.z);
            v2 = glm::vec3(blockPos.x + blockSize, blockPos.y + blockSize, blockPos.z + blockSize);
            v3 = glm::vec3(blockPos.x, blockPos.y + blockSize, blockPos.z + blockSize);
            break;
        case 2: // 前
            v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z);
            v1 = glm::vec3(blockPos.x + blockSize, blockPos.y, blockPos.z);
            v2 = glm::vec3(blockPos.x + blockSize, blockPos.y + blockSize, blockPos.z);
            v3 = glm::vec3(blockPos.x, blockPos.y + blockSize, blockPos.z);
            break;
        case 3: // 后
            v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z + blockSize);
            v1 = glm::vec3(blockPos.x + blockSize, blockPos.y, blockPos.z + blockSize);
            v2 = glm::vec3(blockPos.x + blockSize, blockPos.y + blockSize, blockPos.z + blockSize);
            v3 = glm::vec3(blockPos.x, blockPos.y + blockSize, blockPos.z + blockSize);
            break;
        case 4: // 左
            v0 = glm::vec3(blockPos.x, blockPos.y, blockPos.z);
            v1 = glm::vec3(blockPos.x, blockPos.y, blockPos.z + blockSize);
            v2 = glm::vec3(blockPos.x, blockPos.y + blockSize, blockPos.z + blockSize);
            v3 = glm::vec3(blockPos.x, blockPos.y + blockSize, blockPos.z);
            break;
        case 5: // 右
            v0 = glm::vec3(blockPos.x + blockSize, blockPos.y, blockPos.z);
            v1 = glm::vec3(blockPos.x + blockSize, blockPos.y, blockPos.z + blockSize);
            v2 = glm::vec3(blockPos.x + blockSize, blockPos.y + blockSize, blockPos.z + blockSize);
            v3 = glm::vec3(blockPos.x + blockSize, blockPos.y + blockSize, blockPos.z);
            break;
        }

        // 为当前面添加顶点数据
        vertices[vertexIndex++] = v0.x;
        vertices[vertexIndex++] = v0.y;
        vertices[vertexIndex++] = v0.z;
        vertices[vertexIndex++] = normals[face].x;
        vertices[vertexIndex++] = normals[face].y;
        vertices[vertexIndex++] = normals[face].z;
        vertices[vertexIndex++] = textureCoords[face * 8];
        vertices[vertexIndex++] = textureCoords[face * 8 + 1];

        vertices[vertexIndex++] = v1.x;
        vertices[vertexIndex++] = v1.y;
        vertices[vertexIndex++] = v1.z;
        vertices[vertexIndex++] = normals[face].x;
        vertices[vertexIndex++] = normals[face].y;
        vertices[vertexIndex++] = normals[face].z;
        vertices[vertexIndex++] = textureCoords[face * 8 + 2];
        vertices[vertexIndex++] = textureCoords[face * 8 + 3];

        vertices[vertexIndex++] = v2.x;
        vertices[vertexIndex++] = v2.y;
        vertices[vertexIndex++] = v2.z;
        vertices[vertexIndex++] = normals[face].x;
        vertices[vertexIndex++] = normals[face].y;
        vertices[vertexIndex++] = normals[face].z;
        vertices[vertexIndex++] = textureCoords[face * 8 + 4];
        vertices[vertexIndex++] = textureCoords[face * 8 + 5];

        vertices[vertexIndex++] = v0.x;
        vertices[vertexIndex++] = v0.y;
        vertices[vertexIndex++] = v0.z;
        vertices[vertexIndex++] = normals[face].x;
        vertices[vertexIndex++] = normals[face].y;
        vertices[vertexIndex++] = normals[face].z;
        vertices[vertexIndex++] = textureCoords[face * 8];
        vertices[vertexIndex++] = textureCoords[face * 8 + 1];

        vertices[vertexIndex++] = v2.x;
        vertices[vertexIndex++] = v2.y;
        vertices[vertexIndex++] = v2.z;
        vertices[vertexIndex++] = normals[face].x;
        vertices[vertexIndex++] = normals[face].y;
        vertices[vertexIndex++] = normals[face].z;
        vertices[vertexIndex++] = textureCoords[face * 8 + 4];
        vertices[vertexIndex++] = textureCoords[face * 8 + 5];

        vertices[vertexIndex++] = v3.x;
        vertices[vertexIndex++] = v3.y;
        vertices[vertexIndex++] = v3.z;
        vertices[vertexIndex++] = normals[face].x;
        vertices[vertexIndex++] = normals[face].y;
        vertices[vertexIndex++] = normals[face].z;
        vertices[vertexIndex++] = textureCoords[face * 8 + 6];
        vertices[vertexIndex++] = textureCoords[face * 8 + 7];
    }

    return vertices;
}

// 生成整个chunk的顶点数据
std::vector<float> generateChunkVertices(const chunk &c)
{
    std::vector<float> vertices;
    for (const auto &block : c.c_blocks)
    {
        std::vector<float> blockVertices = generateBlockVertices(block);
        vertices.insert(vertices.end(), blockVertices.begin(), blockVertices.end());
    }
    return vertices;
}

void updateChunk(glm::vec3 viewPosition)
{
    std::vector<float> allVertices;
    GLuint VAO, VBO;
    getViewChunk(viewPosition);
    loadChunk();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 初始化顶点数据
    for (const auto &c : chunks)
    {
        std::vector<float> chunkVertices = generateChunkVertices(c);
        allVertices.insert(allVertices.end(), chunkVertices.begin(), chunkVertices.end());
    }

    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), &allVertices[0], GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, allVertices.size() * sizeof(float));

}
