#include "world.hpp"
#include "noise.hpp"
#include "thread"

void block::GenerateBlockVertices(blockFaces *blockFaces)
{
    /*
    面的次序为下上左右前后（相对于相机视角），每个面有6个点
    0代表原点，1代表正方向，示例：000代表方块内位于0,0,0的点，这个位置的点也用来表示这个方块的位置
    每个点有8个值，分别为x,y,z,uvx，uvy,normalx,normaly,normalz
    */
    // 下
    // 顶点1 (001)
    blockFaces->bottom[0] = this->b_position.x;
    blockFaces->bottom[1] = this->b_position.y;
    blockFaces->bottom[2] = this->b_position.z + 1.0f;
    blockFaces->bottom[3] = 0.0f;  // u
    blockFaces->bottom[4] = 0.0f;  // v
    blockFaces->bottom[5] = 0.0f;  // nx
    blockFaces->bottom[6] = -1.0f; // ny
    blockFaces->bottom[7] = 0.0f;  // nz

    // 顶点2 (000)
    blockFaces->bottom[8] = this->b_position.x;
    blockFaces->bottom[9] = this->b_position.y;
    blockFaces->bottom[10] = this->b_position.z;
    blockFaces->bottom[11] = 0.0f;  // u
    blockFaces->bottom[12] = 1.0f;  // v
    blockFaces->bottom[13] = 0.0f;  // nx
    blockFaces->bottom[14] = -1.0f; // ny
    blockFaces->bottom[15] = 0.0f;  // nz

    // 顶点3 (100)
    blockFaces->bottom[16] = this->b_position.x + 1.0f;
    blockFaces->bottom[17] = this->b_position.y;
    blockFaces->bottom[18] = this->b_position.z;
    blockFaces->bottom[19] = 1.0f;  // u
    blockFaces->bottom[20] = 1.0f;  // v
    blockFaces->bottom[21] = 0.0f;  // nx
    blockFaces->bottom[22] = -1.0f; // ny
    blockFaces->bottom[23] = 0.0f;  // nz

    // 顶点4 (101)
    blockFaces->bottom[24] = this->b_position.x + 1.0f;
    blockFaces->bottom[25] = this->b_position.y;
    blockFaces->bottom[26] = this->b_position.z + 1.0f;
    blockFaces->bottom[27] = 1.0f;  // u
    blockFaces->bottom[28] = 0.0f;  // v
    blockFaces->bottom[29] = 0.0f;  // nx
    blockFaces->bottom[30] = -1.0f; // ny
    blockFaces->bottom[31] = 0.0f;  // nz

    // 上
    // 顶点1 (010)
    blockFaces->top[0] = this->b_position.x;
    blockFaces->top[1] = this->b_position.y + 1.0f;
    blockFaces->top[2] = this->b_position.z;
    blockFaces->top[3] = 0.0f; // u
    blockFaces->top[4] = 0.0f; // v
    blockFaces->top[5] = 0.0f; // nx
    blockFaces->top[6] = 1.0f; // ny
    blockFaces->top[7] = 0.0f; // nz

    // 顶点2 (011)
    blockFaces->top[8] = this->b_position.x;
    blockFaces->top[9] = this->b_position.y + 1.0f;
    blockFaces->top[10] = this->b_position.z + 1.0f;
    blockFaces->top[11] = 0.0f; // u
    blockFaces->top[12] = 1.0f; // v
    blockFaces->top[13] = 0.0f; // nx
    blockFaces->top[14] = 1.0f; // ny
    blockFaces->top[15] = 0.0f; // nz

    // 顶点3 (111)
    blockFaces->top[16] = this->b_position.x + 1.0f;
    blockFaces->top[17] = this->b_position.y + 1.0f;
    blockFaces->top[18] = this->b_position.z + 1.0f;
    blockFaces->top[19] = 1.0f; // u
    blockFaces->top[20] = 1.0f; // v
    blockFaces->top[21] = 0.0f; // nx
    blockFaces->top[22] = 1.0f; // ny
    blockFaces->top[23] = 0.0f; // nz

    // 顶点4 (110)
    blockFaces->top[24] = this->b_position.x + 1.0f;
    blockFaces->top[25] = this->b_position.y + 1.0f;
    blockFaces->top[26] = this->b_position.z;
    blockFaces->top[27] = 1.0f; // u
    blockFaces->top[28] = 0.0f; // v
    blockFaces->top[29] = 0.0f; // nx
    blockFaces->top[30] = 1.0f; // ny
    blockFaces->top[31] = 0.0f; // nz

    // 左
    // 顶点1 (001)
    blockFaces->left[0] = this->b_position.x;
    blockFaces->left[1] = this->b_position.y;
    blockFaces->left[2] = this->b_position.z + 1.0f;
    blockFaces->left[3] = 0.0f;  // u
    blockFaces->left[4] = 0.0f;  // v
    blockFaces->left[5] = -1.0f; // nx
    blockFaces->left[6] = 0.0f;  // ny
    blockFaces->left[7] = 0.0f;  // nz

    // 顶点2 (011)
    blockFaces->left[8] = this->b_position.x;
    blockFaces->left[9] = this->b_position.y + 1.0f;
    blockFaces->left[10] = this->b_position.z + 1.0f;
    blockFaces->left[11] = 0.0f;  // u
    blockFaces->left[12] = 1.0f;  // v
    blockFaces->left[13] = -1.0f; // nx
    blockFaces->left[14] = 0.0f;  // ny
    blockFaces->left[15] = 0.0f;  // nz

    // 顶点3 (010)
    blockFaces->left[16] = this->b_position.x;
    blockFaces->left[17] = this->b_position.y + 1.0f;
    blockFaces->left[18] = this->b_position.z;
    blockFaces->left[19] = 1.0f;  // u
    blockFaces->left[20] = 1.0f;  // v
    blockFaces->left[21] = -1.0f; // nx
    blockFaces->left[22] = 0.0f;  // ny
    blockFaces->left[23] = 0.0f;  // nz

    // 顶点4 (000)
    blockFaces->left[24] = this->b_position.x;
    blockFaces->left[25] = this->b_position.y;
    blockFaces->left[26] = this->b_position.z;
    blockFaces->left[27] = 1.0f;  // u
    blockFaces->left[28] = 0.0f;  // v
    blockFaces->left[29] = -1.0f; // nx
    blockFaces->left[30] = 0.0f;  // ny
    blockFaces->left[31] = 0.0f;  // nz

    // 右
    // 顶点1 (100)
    blockFaces->right[0] = this->b_position.x + 1.0f; // X
    blockFaces->right[1] = this->b_position.y;        // Y
    blockFaces->right[2] = this->b_position.z;        // Z
    blockFaces->right[3] = 0.0f;                      // U
    blockFaces->right[4] = 0.0f;                      // V
    blockFaces->right[5] = 1.0f;                      // Nx
    blockFaces->right[6] = 0.0f;                      // Ny
    blockFaces->right[7] = 0.0f;                      // Nz

    // 顶点2 (110)
    blockFaces->right[8] = this->b_position.x + 1.0f; // X
    blockFaces->right[9] = this->b_position.y + 1.0f; // Y
    blockFaces->right[10] = this->b_position.z;       // Z
    blockFaces->right[11] = 0.0f;                     // U
    blockFaces->right[12] = 1.0f;                     // V
    blockFaces->right[13] = 1.0f;                     // Nx
    blockFaces->right[14] = 0.0f;                     // Ny
    blockFaces->right[15] = 0.0f;                     // Nz

    // 顶点3 (111)
    blockFaces->right[16] = this->b_position.x + 1.0f; // X
    blockFaces->right[17] = this->b_position.y + 1.0f; // Y
    blockFaces->right[18] = this->b_position.z + 1.0f; // Z
    blockFaces->right[19] = 1.0f;                      // U
    blockFaces->right[20] = 1.0f;                      // V
    blockFaces->right[21] = 1.0f;                      // Nx
    blockFaces->right[22] = 0.0f;                      // Ny
    blockFaces->right[23] = 0.0f;                      // Nz

    // 顶点4 (101)
    blockFaces->right[24] = this->b_position.x + 1.0f; // X
    blockFaces->right[25] = this->b_position.y;        // Y
    blockFaces->right[26] = this->b_position.z + 1.0f; // Z
    blockFaces->right[27] = 1.0f;                      // U
    blockFaces->right[28] = 0.0f;                      // V
    blockFaces->right[29] = 1.0f;                      // Nx
    blockFaces->right[30] = 0.0f;                      // Ny
    blockFaces->right[31] = 0.0f;                      // Nz

    // 前
    // 顶点1 (000)
    blockFaces->front[0] = this->b_position.x; // X
    blockFaces->front[1] = this->b_position.y; // Y
    blockFaces->front[2] = this->b_position.z; // Z
    blockFaces->front[3] = 0.0f;               // U
    blockFaces->front[4] = 0.0f;               // V
    blockFaces->front[5] = 0.0f;               // Nx
    blockFaces->front[6] = 0.0f;               // Ny
    blockFaces->front[7] = -1.0f;              // Nz

    // 顶点2 (010)
    blockFaces->front[8] = this->b_position.x;        // X
    blockFaces->front[9] = this->b_position.y + 1.0f; // Y
    blockFaces->front[10] = this->b_position.z;       // Z
    blockFaces->front[11] = 0.0f;                     // U
    blockFaces->front[12] = 1.0f;                     // V
    blockFaces->front[13] = 0.0f;                     // Nx
    blockFaces->front[14] = 0.0f;                     // Ny
    blockFaces->front[15] = -1.0f;                    // Nz

    // 顶点3 (110)
    blockFaces->front[16] = this->b_position.x + 1.0f; // X
    blockFaces->front[17] = this->b_position.y + 1.0f; // Y
    blockFaces->front[18] = this->b_position.z;        // Z
    blockFaces->front[19] = 1.0f;                      // U
    blockFaces->front[20] = 1.0f;                      // V
    blockFaces->front[21] = 0.0f;                      // Nx
    blockFaces->front[22] = 0.0f;                      // Ny
    blockFaces->front[23] = -1.0f;                     // Nz

    // 顶点4 (100)
    blockFaces->front[24] = this->b_position.x + 1.0f; // X
    blockFaces->front[25] = this->b_position.y;        // Y
    blockFaces->front[26] = this->b_position.z;        // Z
    blockFaces->front[27] = 1.0f;                      // U
    blockFaces->front[28] = 0.0f;                      // V
    blockFaces->front[29] = 0.0f;                      // Nx
    blockFaces->front[30] = 0.0f;                      // Ny
    blockFaces->front[31] = -1.0f;                     // Nz

    // 后
    // 顶点1 (101)
    blockFaces->back[0] = this->b_position.x + 1.0f; // X
    blockFaces->back[1] = this->b_position.y;        // Y
    blockFaces->back[2] = this->b_position.z + 1.0f; // Z
    blockFaces->back[3] = 0.0f;                      // U
    blockFaces->back[4] = 0.0f;                      // V
    blockFaces->back[5] = 0.0f;                      // Nx
    blockFaces->back[6] = 0.0f;                      // Ny
    blockFaces->back[7] = 1.0f;                      // Nz

    // 顶点2 (111)
    blockFaces->back[8] = this->b_position.x + 1.0f;  // X
    blockFaces->back[9] = this->b_position.y + 1.0f;  // Y
    blockFaces->back[10] = this->b_position.z + 1.0f; // Z
    blockFaces->back[11] = 0.0f;                      // U
    blockFaces->back[12] = 1.0f;                      // V
    blockFaces->back[13] = 0.0f;                      // Nx
    blockFaces->back[14] = 0.0f;                      // Ny
    blockFaces->back[15] = 1.0f;                      // Nz

    // 顶点3 (011)
    blockFaces->back[16] = this->b_position.x;        // X
    blockFaces->back[17] = this->b_position.y + 1.0f; // Y
    blockFaces->back[18] = this->b_position.z + 1.0f; // Z
    blockFaces->back[19] = 1.0f;                      // U
    blockFaces->back[20] = 1.0f;                      // V
    blockFaces->back[21] = 0.0f;                      // Nx
    blockFaces->back[22] = 0.0f;                      // Ny
    blockFaces->back[23] = 1.0f;                      // Nz

    // 顶点4 (001)
    blockFaces->back[24] = this->b_position.x;        // X
    blockFaces->back[25] = this->b_position.y;        // Y
    blockFaces->back[26] = this->b_position.z + 1.0f; // Z
    blockFaces->back[27] = 1.0f;                      // U
    blockFaces->back[28] = 0.0f;                      // V
    blockFaces->back[29] = 0.0f;                      // Nx
    blockFaces->back[30] = 0.0f;                      // Ny
    blockFaces->back[31] = 1.0f;                      // Nz
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
                if (ii <= hei||ii<=10)
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

template <class C, int N>
worldVector<C, N>::worldVector()
{
    this->wv = new C[N];
    this->len = N;
}
template <class C, int N>
C &worldVector<C, N>::operator[](size_t index)
{
    if (index > this->len)
    {
        C *temp = this->wv;
        this->wv = new C[index + 2];
        for (int i = 0; i < this->len; i++)
        {
            this->wv[i] = temp[i];
        }
        delete[] temp;
        this->len = index + 2;
    }
    return this->wv[index];
}