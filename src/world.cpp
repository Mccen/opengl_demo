#include "world.hpp"

void block::GenBottomVertices()
{

    // 下
    // 顶点1 (001)
    b_mesh[face::bottom].positions[0] = this->b_position.x;
    b_mesh[face::bottom].positions[1] = this->b_position.y;
    b_mesh[face::bottom].positions[2] = this->b_position.z + 1.0f;
    // 顶点2 (000)
    b_mesh[face::bottom].positions[3] = this->b_position.x;
    b_mesh[face::bottom].positions[4] = this->b_position.y;
    b_mesh[face::bottom].positions[5] = this->b_position.z;
    // 顶点3 (100)
    b_mesh[face::bottom].positions[6] = this->b_position.x + 1.0f;
    b_mesh[face::bottom].positions[7] = this->b_position.y;
    b_mesh[face::bottom].positions[8] = this->b_position.z;
    // 顶点4 (101)
    b_mesh[face::bottom].positions[9] = this->b_position.x + 1.0f;
    b_mesh[face::bottom].positions[10] = this->b_position.y;
    b_mesh[face::bottom].positions[11] = this->b_position.z + 1.0f;

    b_mesh[face::bottom].texCoords[0] = 0.0f; // u
    b_mesh[face::bottom].texCoords[1] = 0.0f; // v
    b_mesh[face::bottom].texCoords[2] = 0.0f; // u
    b_mesh[face::bottom].texCoords[3] = 1.0f; // v
    b_mesh[face::bottom].texCoords[4] = 1.0f; // u
    b_mesh[face::bottom].texCoords[5] = 1.0f; // v
    b_mesh[face::bottom].texCoords[6] = 1.0f; // u
    b_mesh[face::bottom].texCoords[7] = 0.0f; // v

    // 法线
    b_mesh[face::bottom].normals[0] = 0.0f;  // nx
    b_mesh[face::bottom].normals[1] = -1.0f; // ny
    b_mesh[face::bottom].normals[2] = 0.0f;  // nz
}
void block::GenTopVertices()
{
    // 上
    // 顶点1 (010)
    b_mesh[face::top].positions[0] = this->b_position.x;
    b_mesh[face::top].positions[1] = this->b_position.y + 1.0f;
    b_mesh[face::top].positions[2] = this->b_position.z;
    // 顶点2 (011)
    b_mesh[face::top].positions[3] = this->b_position.x;
    b_mesh[face::top].positions[4] = this->b_position.y + 1.0f;
    b_mesh[face::top].positions[5] = this->b_position.z + 1.0f;
    // 顶点3 (111)
    b_mesh[face::top].positions[6] = this->b_position.x + 1.0f;
    b_mesh[face::top].positions[7] = this->b_position.y + 1.0f;
    b_mesh[face::top].positions[8] = this->b_position.z + 1.0f;
    // 顶点4 (110)
    b_mesh[face::top].positions[9] = this->b_position.x + 1.0f;
    b_mesh[face::top].positions[10] = this->b_position.y + 1.0f;
    b_mesh[face::top].positions[11] = this->b_position.z;

    b_mesh[face::top].texCoords[0] = 0.0f; // u
    b_mesh[face::top].texCoords[1] = 0.0f; // v
    b_mesh[face::top].texCoords[2] = 0.0f; // u
    b_mesh[face::top].texCoords[3] = 1.0f; // v
    b_mesh[face::top].texCoords[4] = 1.0f; // u
    b_mesh[face::top].texCoords[5] = 1.0f; // v
    b_mesh[face::top].texCoords[6] = 1.0f; // u
    b_mesh[face::top].texCoords[7] = 0.0f; // v

    // 法线
    b_mesh[face::top].normals[0] = 0.0f; // nx
    b_mesh[face::top].normals[1] = 1.0f; // ny
    b_mesh[face::top].normals[2] = 0.0f; // nz
}
void block::GenLeftVertices()
{
    // 左
    // 顶点1 (001)
    b_mesh[face::left].positions[0] = this->b_position.x;
    b_mesh[face::left].positions[1] = this->b_position.y;
    b_mesh[face::left].positions[2] = this->b_position.z + 1.0f;
    // 顶点2 (011)
    b_mesh[face::left].positions[3] = this->b_position.x;
    b_mesh[face::left].positions[4] = this->b_position.y + 1.0f;
    b_mesh[face::left].positions[5] = this->b_position.z + 1.0f;
    // 顶点3 (010)
    b_mesh[face::left].positions[6] = this->b_position.x;
    b_mesh[face::left].positions[7] = this->b_position.y + 1.0f;
    b_mesh[face::left].positions[8] = this->b_position.z;
    // 顶点4 (000)
    b_mesh[face::left].positions[9] = this->b_position.x;
    b_mesh[face::left].positions[10] = this->b_position.y;
    b_mesh[face::left].positions[11] = this->b_position.z;

    b_mesh[face::left].texCoords[0] = 0.0f; // u
    b_mesh[face::left].texCoords[1] = 0.0f; // v
    b_mesh[face::left].texCoords[2] = 0.0f; // u
    b_mesh[face::left].texCoords[3] = 1.0f; // v
    b_mesh[face::left].texCoords[4] = 1.0f; // u
    b_mesh[face::left].texCoords[5] = 1.0f; // v
    b_mesh[face::left].texCoords[6] = 1.0f; // u
    b_mesh[face::left].texCoords[7] = 0.0f; // v

    // 法线
    b_mesh[face::left].normals[0] = -1.0f; // nx
    b_mesh[face::left].normals[1] = 0.0f;  // ny
    b_mesh[face::left].normals[2] = 0.0f;  // nz
}
void block::GenRightVertices()
{
    // 右
    // 顶点1 (100)
    b_mesh[face::right].positions[0] = this->b_position.x + 1.0f;
    b_mesh[face::right].positions[1] = this->b_position.y;
    b_mesh[face::right].positions[2] = this->b_position.z;
    // 顶点2 (110)
    b_mesh[face::right].positions[3] = this->b_position.x + 1.0f;
    b_mesh[face::right].positions[4] = this->b_position.y + 1.0f;
    b_mesh[face::right].positions[5] = this->b_position.z;
    // 顶点3 (111)
    b_mesh[face::right].positions[6] = this->b_position.x + 1.0f;
    b_mesh[face::right].positions[7] = this->b_position.y + 1.0f;
    b_mesh[face::right].positions[8] = this->b_position.z + 1.0f;
    // 顶点4 (101)
    b_mesh[face::right].positions[9] = this->b_position.x + 1.0f;
    b_mesh[face::right].positions[10] = this->b_position.y;
    b_mesh[face::right].positions[11] = this->b_position.z + 1.0f;

    b_mesh[face::right].texCoords[0] = 0.0f; // u
    b_mesh[face::right].texCoords[1] = 0.0f; // v
    b_mesh[face::right].texCoords[2] = 0.0f; // u
    b_mesh[face::right].texCoords[3] = 1.0f; // v
    b_mesh[face::right].texCoords[4] = 1.0f; // u
    b_mesh[face::right].texCoords[5] = 1.0f; // v
    b_mesh[face::right].texCoords[6] = 1.0f; // u
    b_mesh[face::right].texCoords[7] = 0.0f; // v

    // 法线
    b_mesh[face::right].normals[0] = 1.0f; // nx
    b_mesh[face::right].normals[1] = 0.0f; // ny
    b_mesh[face::right].normals[2] = 0.0f;
}
void block::GenFrontVertices()
{
    // 前
    // 顶点1 (000)
    b_mesh[face::front].positions[0] = this->b_position.x;
    b_mesh[face::front].positions[1] = this->b_position.y;
    b_mesh[face::front].positions[2] = this->b_position.z;
    // 顶点2 (010)
    b_mesh[face::front].positions[3] = this->b_position.x;
    b_mesh[face::front].positions[4] = this->b_position.y + 1.0f;
    b_mesh[face::front].positions[5] = this->b_position.z;
    // 顶点3 (110)
    b_mesh[face::front].positions[6] = this->b_position.x + 1.0f;
    b_mesh[face::front].positions[7] = this->b_position.y + 1.0f;
    b_mesh[face::front].positions[8] = this->b_position.z;
    // 顶点4 (100)
    b_mesh[face::front].positions[9] = this->b_position.x + 1.0f;
    b_mesh[face::front].positions[10] = this->b_position.y;
    b_mesh[face::front].positions[11] = this->b_position.z;

    b_mesh[face::front].texCoords[0] = 0.0f; // u
    b_mesh[face::front].texCoords[1] = 0.0f; // v
    b_mesh[face::front].texCoords[2] = 0.0f; // u
    b_mesh[face::front].texCoords[3] = 1.0f; // v
    b_mesh[face::front].texCoords[4] = 1.0f; // u
    b_mesh[face::front].texCoords[5] = 1.0f; // v
    b_mesh[face::front].texCoords[6] = 1.0f; // u
    b_mesh[face::front].texCoords[7] = 0.0f; // v

    // 法线
    b_mesh[face::front].normals[0] = 0.0f;  // nx
    b_mesh[face::front].normals[1] = 0.0f;  // ny
    b_mesh[face::front].normals[2] = -1.0f; // nz
}
void block::GenBackVertices()
{
    // 后
    // 顶点1 (101)
    b_mesh[face::back].positions[0] = this->b_position.x + 1.0f;
    b_mesh[face::back].positions[1] = this->b_position.y;
    b_mesh[face::back].positions[2] = this->b_position.z + 1.0f;
    // 顶点2 (111)
    b_mesh[face::back].positions[3] = this->b_position.x + 1.0f;
    b_mesh[face::back].positions[4] = this->b_position.y + 1.0f;
    b_mesh[face::back].positions[5] = this->b_position.z + 1.0f;
    // 顶点3 (011)
    b_mesh[face::back].positions[6] = this->b_position.x;
    b_mesh[face::back].positions[7] = this->b_position.y + 1.0f;
    b_mesh[face::back].positions[8] = this->b_position.z+ 1.0f;
    // 顶点4 (001)
    b_mesh[face::back].positions[9] = this->b_position.x;
    b_mesh[face::back].positions[10] = this->b_position.y;
    b_mesh[face::back].positions[11] = this->b_position.z+ 1.0f;

    b_mesh[face::back].texCoords[0] = 0.0f; // u
    b_mesh[face::back].texCoords[1] = 0.0f; // v
    b_mesh[face::back].texCoords[2] = 0.0f; // u
    b_mesh[face::back].texCoords[3] = 1.0f; // v
    b_mesh[face::back].texCoords[4] = 1.0f; // u
    b_mesh[face::back].texCoords[5] = 1.0f; // v
    b_mesh[face::back].texCoords[6] = 1.0f; // u
    b_mesh[face::back].texCoords[7] = 0.0f; // v

    // 法线
    b_mesh[face::back].normals[0] = 0.0f;  // nx
    b_mesh[face::back].normals[1] = 0.0f;  // ny
    b_mesh[face::back].normals[2] = 1.0f; // nz
}

block::block(glm::vec3 Pos, type type)
{
    this->b_position = Pos;
    this->b_type = type;
}

void chunk::createBlock()
{
    Noise noise(this->c_position);
    glm::vec2 temp = glm::vec2(this->c_position.x * 16, this->c_position.y * 16);
    int i = 0;
    for (int x = temp.x; x < temp.x + 16; x++)
    {
        int j = 0;
        for (int z = temp.y; z < temp.y + 16; z++)
        {
            for (int ii = 0; ii < this->c_height; ii++)
            {
                int hei = static_cast<int>(noise.finNum[i][j] * this->c_height);
                if (ii <= hei || ii <= 10)
                {
                    block temp(glm::vec3(x, ii, z), stone);
                    this->c_blocks[i][j][ii] = temp;
                }
                else
                {
                    block temp(glm::vec3(x, ii, z), air);
                    this->c_blocks[i][j][ii] = temp;
                }
            }
            j++;
        }
        i++;
    }
    this->hasBlock = true;
}
chunk::chunk(glm::vec2 chunkposition)
{
    this->c_position = chunkposition;
    if (!hasBlock)
        createBlock();
}

void World::updateCameraChunk(glm::vec3 viewPosition)
{
    getWorld().cameraChunk.x = floor(viewPosition.x / 16);
    getWorld().cameraChunk.y = floor(viewPosition.z / 16);
}
bool World::checkhasChunk(glm::ivec2 position)
{
    return this->memoryChunks[position.x][position.y].hasBlock;
}
glm::ivec2 World::changeCoordToIndex(glm::vec2 position)
{
    glm::vec2 tempIndex;
    if (position.x <= 0)
    {
        if (position.y <= 0)
        {
            tempIndex.x = 2 * abs(position.x);
            tempIndex.y = 2 * abs(position.y);
        }
        else
        {
            tempIndex.x = 2 * abs(position.x);
            tempIndex.y = 2 * abs(position.y) - 1;
        }
    }
    else
    {
        if (position.y <= 0)
        {
            tempIndex.x = 2 * abs(position.x) - 1;
            tempIndex.y = 2 * abs(position.y);
        }
        else
        {
            tempIndex.x = 2 * abs(position.x) - 1;
            tempIndex.y = 2 * abs(position.y) - 1;
        }
    }
    return tempIndex;
}
void World::createChunk(glm::ivec2 position)
{
    if (!checkhasChunk(changeCoordToIndex(position)))
    {
        chunk temp(position);
        glm::ivec2 index = changeCoordToIndex(position);
        this->memoryChunks[index.x][index.y] = temp;
    }
}
void World::makeVAO()
{
}
void World::renderChunk()
{
    glBindVertexArray(worldVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void World::loadViewChunk()
{
    int i = 0;
    for (int x = (cameraChunk.x - viewDistance); x <= (cameraChunk.x + viewDistance); x++)
    {
        int ii = 0;
        for (int z = (cameraChunk.y - viewDistance); z <= (cameraChunk.y + viewDistance); z++)
        {
            createChunk(glm::ivec2(x, z));
            glm::ivec2 temp = changeCoordToIndex(glm::vec2(x, z));
            this->viewChunks[i * (viewDistance * 2 + 1) + ii] = this->memoryChunks[temp.x][temp.y];
            ii++;
        }
        i++;
    }
    this->isFirst = false;
}
void World::saveChunk()
{
    if (this->changeCoordToIndex(cameraChunk).x >= memorySize - 8 || this->changeCoordToIndex(cameraChunk).y >= memorySize - 8)
    {
    }
}
World::World()
{
    loadViewChunk();
}
