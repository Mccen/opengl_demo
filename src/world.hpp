#include <unordered_set>
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
struct block
{
    glm::vec3 b_position;
    type b_type;
    GLuint b_tex;
    block(glm::vec3 position, type type, GLuint tex)
    {
        b_position = position;
        b_type = type;
        b_tex = tex;
        GLfloat vertices[] = {
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

            // 后面
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

            // 上面
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            // 下面
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            // 左面
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            // 右面
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
            };
    }
};
struct chunk
{
    // chunk的位置
    glm::vec2 c_position;
    // 是否生成过
    bool c_isGenerated;
    // 区块边长
    int c_size = 16;
    std::vector<block> c_blocks;
    void createBlock()
    {
        for (int x = this->c_position.x * c_size; x < c_size; x++)
        {
            for (int z = this->c_position.y * c_size; z < c_size; z++)
            {
                this->c_blocks.push_back(block(glm::vec3(x, 0, z), stone, 0));
            }
        }
    }
};
std::vector<chunk> chunks;
chunk viewChunk;
int chunkNum = 1;

void getViewChunk(glm::vec3 viewPosition)
{
    glm::vec2 chunkPosition;
    chunkPosition.x = viewPosition.x / 16.0f;
    chunkPosition.y = viewPosition.z / 16.0f;
    viewChunk.c_position = chunkPosition;
    viewChunk.c_isGenerated = true;
}
bool hasChunk(chunk temp)
{
    if (chunkI.find(temp) != chunkI.end())
        return true;
    else
        return false;
}
void createChunk()
{
    chunk tempChunk;
    for (int i = viewChunk.c_position.x - chunkNum; i < viewChunk.c_position.x + chunkNum; i++)
    {
        for (int ii = viewChunk.c_position.y - chunkNum; ii < viewChunk.c_position.y + chunkNum; ii++)
        {
            tempChunk.c_position.x = i;
            tempChunk.c_position.y = ii;
            if (!hasChunk(tempChunk))
            {
                tempChunk.createBlock();
                chunkI.insert(tempChunk);
            }
            chunks.push_back(tempChunk);
        }
    }
}
void randerChunk()
{
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, chunks.size() * sizeof(block), &chunks[0], GL_STATIC_DRAW);
}