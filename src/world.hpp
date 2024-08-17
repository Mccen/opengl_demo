#include <map>
std::vector<float> allb_vertices;
GLuint worldVAO, worldVBO;
bool isInit = false;
// 直线可视区块距离（不包括脚下的)
int chunkNum = 3;
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
const float blockSize = 1.0f;
const glm::vec3 normals[] = {
    glm::vec3(0.0f, -1.0f, 0.0f), // 下
    glm::vec3(0.0f, 1.0f, 0.0f),  // 上
    glm::vec3(0.0f, 0.0f, 1.0f),  // 前
    glm::vec3(0.0f, 0.0f, -1.0f), // 后
    glm::vec3(-1.0f, 0.0f, 0.0f), // 左
    glm::vec3(1.0f, 0.0f, 0.0f)   // 右
};

const float textureCoords[] = {
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

struct block
{
    glm::vec3 b_position;
    type b_type;
    std::vector<GLfloat> b_vertices;
    block(glm::vec3 blockPos, type type)
    {
        b_position = blockPos;
        b_type = type;
        b_vertices.resize(36 * 8);

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
            b_vertices[vertexIndex++] = v0.x;
            b_vertices[vertexIndex++] = v0.y;
            b_vertices[vertexIndex++] = v0.z;
            b_vertices[vertexIndex++] = normals[face].x;
            b_vertices[vertexIndex++] = normals[face].y;
            b_vertices[vertexIndex++] = normals[face].z;
            b_vertices[vertexIndex++] = textureCoords[face * 8];
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 1];

            b_vertices[vertexIndex++] = v1.x;
            b_vertices[vertexIndex++] = v1.y;
            b_vertices[vertexIndex++] = v1.z;
            b_vertices[vertexIndex++] = normals[face].x;
            b_vertices[vertexIndex++] = normals[face].y;
            b_vertices[vertexIndex++] = normals[face].z;
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 2];
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 3];

            b_vertices[vertexIndex++] = v2.x;
            b_vertices[vertexIndex++] = v2.y;
            b_vertices[vertexIndex++] = v2.z;
            b_vertices[vertexIndex++] = normals[face].x;
            b_vertices[vertexIndex++] = normals[face].y;
            b_vertices[vertexIndex++] = normals[face].z;
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 4];
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 5];

            b_vertices[vertexIndex++] = v0.x;
            b_vertices[vertexIndex++] = v0.y;
            b_vertices[vertexIndex++] = v0.z;
            b_vertices[vertexIndex++] = normals[face].x;
            b_vertices[vertexIndex++] = normals[face].y;
            b_vertices[vertexIndex++] = normals[face].z;
            b_vertices[vertexIndex++] = textureCoords[face * 8];
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 1];

            b_vertices[vertexIndex++] = v2.x;
            b_vertices[vertexIndex++] = v2.y;
            b_vertices[vertexIndex++] = v2.z;
            b_vertices[vertexIndex++] = normals[face].x;
            b_vertices[vertexIndex++] = normals[face].y;
            b_vertices[vertexIndex++] = normals[face].z;
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 4];
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 5];

            b_vertices[vertexIndex++] = v3.x;
            b_vertices[vertexIndex++] = v3.y;
            b_vertices[vertexIndex++] = v3.z;
            b_vertices[vertexIndex++] = normals[face].x;
            b_vertices[vertexIndex++] = normals[face].y;
            b_vertices[vertexIndex++] = normals[face].z;
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 6];
            b_vertices[vertexIndex++] = textureCoords[face * 8 + 7];
        }
    }
};
struct chunk
{
    // chunk的位置
    glm::vec2 c_position;
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
    }
};

// 自定义重载<
struct Vec2Compare
{
    bool operator()(const glm::vec2 &lhs, const glm::vec2 &rhs) const
    {
        if (lhs.x < rhs.x)
            return true;
        if (lhs.x == rhs.x && lhs.y < rhs.y)
            return true;
        return false;
    }
};
//记录相机所在区块
chunk viewChunk;
// 创建chunk查找结构，留档已创建的区块
std::map<glm::vec2, chunk, Vec2Compare> chunkMap;
// 储存的要使用的chunk
std::vector<chunk> chunks;

void getViewChunk(glm::vec3 viewPosition)
{
    glm::vec2 chunkPosition;
    chunkPosition.x = static_cast<int>(std::floor(viewPosition.x / 16.0f));
    chunkPosition.y = static_cast<int>(std::floor(viewPosition.z / 16.0f));
    viewChunk.c_position = chunkPosition;
    std::cout << "viewChunk: " << viewChunk.c_position.x << " " << viewChunk.c_position.y << std::endl;
}

void loadChunk()
{
    // chunkHas.clear();
    chunks.clear();
    for (int i = viewChunk.c_position.x - chunkNum; i <= viewChunk.c_position.x + chunkNum; i++)
    {
        for (int ii = viewChunk.c_position.y - chunkNum; ii <= viewChunk.c_position.y + chunkNum; ii++)
        {
            if (i == viewChunk.c_position.x - chunkNum && ii == viewChunk.c_position.y - chunkNum)
            {
                continue;
            }
            if (i == viewChunk.c_position.x + chunkNum && ii == viewChunk.c_position.y - chunkNum)
            {
                continue;
            }
            if (i == viewChunk.c_position.x - chunkNum && ii == viewChunk.c_position.y + chunkNum)
            {
                continue;
            }
            if (i == viewChunk.c_position.x + chunkNum && ii == viewChunk.c_position.y + chunkNum)
            {
                continue;
            }
            chunk tempChunk(glm::vec2(i, ii));
            if (chunkMap.find(tempChunk.c_position) == chunkMap.end())
            {
                tempChunk.createBlock();
                chunkMap[tempChunk.c_position] = tempChunk;
                chunks.push_back(chunkMap[tempChunk.c_position]);
                std::cout << "new" << std::endl;
            }
            else
            {
                chunks.push_back(chunkMap[tempChunk.c_position]);
            }
        }
    }
}

// 生成整个chunk的顶点数据
std::vector<float> chunkvertices;
std::vector<float> generateChunkvertices(const chunk &c)
{
    chunkvertices.clear();
    chunkvertices.reserve(c.c_blocks.size() * 36 * 8);
    for (const auto &block : c.c_blocks)
    {
        chunkvertices.insert(chunkvertices.end(), block.b_vertices.begin(), block.b_vertices.end());
    }
    return chunkvertices;
}

void updateworldVAO(glm::vec3 viewPosition)
{
    getViewChunk(viewPosition);
    loadChunk();
    if (!isInit)
    {
        glGenVertexArrays(1, &worldVAO);
        glGenBuffers(1, &worldVBO);
        glBindVertexArray(worldVAO);
        glBindBuffer(GL_ARRAY_BUFFER, worldVBO);
        isInit = true;
    }
    glBindVertexArray(worldVAO);
    // 初始化顶点数据
    allb_vertices.clear();
    for (const auto &c : chunks)
    {
        std::vector<float> chunkb_vertices = generateChunkvertices(c);
        allb_vertices.insert(allb_vertices.end(), chunkb_vertices.begin(), chunkb_vertices.end());
    }

    glBufferData(GL_ARRAY_BUFFER, allb_vertices.size() * sizeof(float), &allb_vertices[0], GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}
void renderChunk()
{
    glDrawArrays(GL_TRIANGLES, 0, allb_vertices.size() * sizeof(float));
}
