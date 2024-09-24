#include "world.hpp"
#include "noise.hpp"
#include "headerLists.hpp"
#include <map>

std::vector<float> allb_vertices; // 储存所有方块信息
GLuint worldVAO, worldVBO;
bool isInit = false;
// 直线可视区块距离（不包括脚下的）
int chunkNum = 1;
const float blockSize = 1.0f;
// 记录相机所在区块
chunk viewChunk;
// 创建chunk查找结构，留档已创建的区块
std::map<glm::vec2, chunk, Vec2Compare> chunkMap;
// 储存的要使用的chunk
std::vector<chunk> chunks;
// 生成整个chunk的顶点数据
std::vector<float> chunkvertices;
const glm::vec3 normals[] = {
    glm::vec3(0.0f, -1.0f, 0.0f), // 下
    glm::vec3(0.0f, 1.0f, 0.0f),  // 上
    glm::vec3(0.0f, 0.0f, 1.0f),  // 前
    glm::vec3(0.0f, 0.0f, -1.0f), // 后
    glm::vec3(-1.0f, 0.0f, 0.0f), // 左
    glm::vec3(1.0f, 0.0f, 0.0f)   // 右
};

const float textureCoords[] = {0.0f, 0.0f, // 下
                               1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

                               0.0f, 0.0f, // 上
                               1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

                               0.0f, 0.0f, // 前
                               1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

                               0.0f, 0.0f, // 后
                               1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

                               0.0f, 0.0f, // 左
                               1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

                               0.0f, 0.0f, // 右
                               1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

block::block(glm::vec3 Pos, type type)
{
  b_position = Pos;
  b_type = type;
  b_vertices.resize(36 * 8);
}
chunk::chunk() {}
chunk::chunk(glm::vec2 position) : c_position(position)
{
  c_blocks.reserve(c_height * c_size * c_size * sizeof(block));
}
void chunk::createBlock()
{
  std::vector<block> temp_blocks;
  Noise noise(this->c_position);
  temp_blocks.reserve(c_height * c_size * c_size * sizeof(block));
  for (int x = this->c_position.x * c_size;
       x < (this->c_position.x + 1) * c_size; x++)
  {
    for (int z = this->c_position.y * c_size;
         z < (this->c_position.y + 1) * c_size; z++)
    {
      temp_blocks.push_back(block(glm::vec3(x, noise.finNum[x][z], z), stone));
    }
  }
  for (int x = this->c_position.x * c_size;
       x < (this->c_position.x + 1) * c_size; x++)
  {
    for (int z = this->c_position.y * c_size;
         z < (this->c_position.y + 1) * c_size; z++)
    {
      block temp = block(glm::vec3(x, noise.finNum[x][z], z), stone);
      int vertexIndex = 0;
      // 定义邻居位置偏移
      glm::vec3 neighborOffsets[6] = {
          glm::vec3(0.0f, -1.0f, 0.0f), // 下
          glm::vec3(0.0f, 1.0f, 0.0f),  // 上
          glm::vec3(0.0f, 0.0f, -1.0f), // 前
          glm::vec3(0.0f, 0.0f, 1.0f),  // 后
          glm::vec3(-1.0f, 0.0f, 0.0f), // 左
          glm::vec3(1.0f, 0.0f, 0.0f)   // 右
      };

      // 遍历每个面，检查是否需要剔除
      for (int face = 0; face < 6; face++)
      {
        // 获取相邻方块位置
        glm::vec3 neighborPos = temp.b_position + neighborOffsets[face];
        bool hasNeighbor = false;
        for (const auto &b : temp_blocks)
        {
          if (b.b_position == neighborPos && b.b_type != air)
          {
            hasNeighbor = true;
            break;
          }
          else
          {
            hasNeighbor = false;
          }
        }
        // 如果邻居存在且不是透明方块，剔除当前面
        if (hasNeighbor)
        {
          continue; // 面被遮挡，不生成顶点
        }
        glm::vec3 v0, v1, v2, v3;
        switch (face)
        {
        case 0: // 下
          v0 = glm::vec3(temp.b_position.x, temp.b_position.y,
                         temp.b_position.z);
          v1 = glm::vec3(temp.b_position.x + blockSize, temp.b_position.y,
                         temp.b_position.z);
          v2 = glm::vec3(temp.b_position.x + blockSize, temp.b_position.y,
                         temp.b_position.z + blockSize);
          v3 = glm::vec3(temp.b_position.x, temp.b_position.y,
                         temp.b_position.z + blockSize);
          break;
        case 1: // 上
          v0 = glm::vec3(temp.b_position.x, temp.b_position.y + blockSize,
                         temp.b_position.z);
          v1 = glm::vec3(temp.b_position.x + blockSize,
                         temp.b_position.y + blockSize, temp.b_position.z);
          v2 = glm::vec3(temp.b_position.x + blockSize,
                         temp.b_position.y + blockSize,
                         temp.b_position.z + blockSize);
          v3 = glm::vec3(temp.b_position.x, temp.b_position.y + blockSize,
                         temp.b_position.z + blockSize);
          break;
        case 2: // 前
          v0 = glm::vec3(temp.b_position.x, temp.b_position.y,
                         temp.b_position.z);
          v1 = glm::vec3(temp.b_position.x + blockSize, temp.b_position.y,
                         temp.b_position.z);
          v2 = glm::vec3(temp.b_position.x + blockSize,
                         temp.b_position.y + blockSize, temp.b_position.z);
          v3 = glm::vec3(temp.b_position.x, temp.b_position.y + blockSize,
                         temp.b_position.z);
          break;
        case 3: // 后
          v0 = glm::vec3(temp.b_position.x, temp.b_position.y,
                         temp.b_position.z + blockSize);
          v1 = glm::vec3(temp.b_position.x + blockSize, temp.b_position.y,
                         temp.b_position.z + blockSize);
          v2 = glm::vec3(temp.b_position.x + blockSize,
                         temp.b_position.y + blockSize,
                         temp.b_position.z + blockSize);
          v3 = glm::vec3(temp.b_position.x, temp.b_position.y + blockSize,
                         temp.b_position.z + blockSize);
          break;
        case 4: // 左
          v0 = glm::vec3(temp.b_position.x, temp.b_position.y,
                         temp.b_position.z);
          v1 = glm::vec3(temp.b_position.x, temp.b_position.y,
                         temp.b_position.z + blockSize);
          v2 = glm::vec3(temp.b_position.x, temp.b_position.y + blockSize,
                         temp.b_position.z + blockSize);
          v3 = glm::vec3(temp.b_position.x, temp.b_position.y + blockSize,
                         temp.b_position.z);
          break;
        case 5: // 右
          v0 = glm::vec3(temp.b_position.x + blockSize, temp.b_position.y,
                         temp.b_position.z);
          v1 = glm::vec3(temp.b_position.x + blockSize, temp.b_position.y,
                         temp.b_position.z + blockSize);
          v2 = glm::vec3(temp.b_position.x + blockSize,
                         temp.b_position.y + blockSize,
                         temp.b_position.z + blockSize);
          v3 = glm::vec3(temp.b_position.x + blockSize,
                         temp.b_position.y + blockSize, temp.b_position.z);
          break;
        }
        // 为当前面添加顶点数据
        temp.b_vertices[vertexIndex++] = v0.x;
        temp.b_vertices[vertexIndex++] = v0.y;
        temp.b_vertices[vertexIndex++] = v0.z;
        temp.b_vertices[vertexIndex++] = normals[face].x;
        temp.b_vertices[vertexIndex++] = normals[face].y;
        temp.b_vertices[vertexIndex++] = normals[face].z;
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8];
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 1];

        temp.b_vertices[vertexIndex++] = v1.x;
        temp.b_vertices[vertexIndex++] = v1.y;
        temp.b_vertices[vertexIndex++] = v1.z;
        temp.b_vertices[vertexIndex++] = normals[face].x;
        temp.b_vertices[vertexIndex++] = normals[face].y;
        temp.b_vertices[vertexIndex++] = normals[face].z;
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 2];
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 3];

        temp.b_vertices[vertexIndex++] = v2.x;
        temp.b_vertices[vertexIndex++] = v2.y;
        temp.b_vertices[vertexIndex++] = v2.z;
        temp.b_vertices[vertexIndex++] = normals[face].x;
        temp.b_vertices[vertexIndex++] = normals[face].y;
        temp.b_vertices[vertexIndex++] = normals[face].z;
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 4];
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 5];

        temp.b_vertices[vertexIndex++] = v0.x;
        temp.b_vertices[vertexIndex++] = v0.y;
        temp.b_vertices[vertexIndex++] = v0.z;
        temp.b_vertices[vertexIndex++] = normals[face].x;
        temp.b_vertices[vertexIndex++] = normals[face].y;
        temp.b_vertices[vertexIndex++] = normals[face].z;
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8];
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 1];

        temp.b_vertices[vertexIndex++] = v2.x;
        temp.b_vertices[vertexIndex++] = v2.y;
        temp.b_vertices[vertexIndex++] = v2.z;
        temp.b_vertices[vertexIndex++] = normals[face].x;
        temp.b_vertices[vertexIndex++] = normals[face].y;
        temp.b_vertices[vertexIndex++] = normals[face].z;
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 4];
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 5];

        temp.b_vertices[vertexIndex++] = v3.x;
        temp.b_vertices[vertexIndex++] = v3.y;
        temp.b_vertices[vertexIndex++] = v3.z;
        temp.b_vertices[vertexIndex++] = normals[face].x;
        temp.b_vertices[vertexIndex++] = normals[face].y;
        temp.b_vertices[vertexIndex++] = normals[face].z;
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 6];
        temp.b_vertices[vertexIndex++] = textureCoords[face * 8 + 7];
      }
      c_blocks.push_back(temp);
    }
  }
  temp_blocks.clear();
}

void getViewChunk(glm::vec3 viewPosition)
{
  glm::vec2 chunkPosition;
  chunkPosition.x = static_cast<int>(std::floor(viewPosition.x / 16.0f));
  chunkPosition.y = static_cast<int>(std::floor(viewPosition.z / 16.0f));
  viewChunk.c_position = chunkPosition;
}

void loadChunk()
{
  chunks.clear();
  for (int i = viewChunk.c_position.x - chunkNum;
       i <= viewChunk.c_position.x + chunkNum; i++)
  {
    for (int ii = viewChunk.c_position.y - chunkNum;
         ii <= viewChunk.c_position.y + chunkNum; ii++)
    {
      chunk tempChunk(glm::vec2(i, ii));
      if (chunkMap.find(tempChunk.c_position) == chunkMap.end())
      {
        tempChunk.createBlock();
        chunkMap[tempChunk.c_position] = tempChunk;
        chunks.push_back(chunkMap[tempChunk.c_position]);
      }
      else
      {
        chunks.push_back(chunkMap[tempChunk.c_position]);
      }
    }
  }
}

std::vector<float> generateChunkvertices(const chunk &c)
{
  chunkvertices.reserve(c.c_blocks.size());
  chunkvertices.clear();
  for (const auto &block : c.c_blocks)
  {
    chunkvertices.insert(chunkvertices.end(), block.b_vertices.begin(),
                         block.b_vertices.end());
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
    allb_vertices.insert(allb_vertices.end(), chunkb_vertices.begin(),
                         chunkb_vertices.end());
  }

  glBufferData(GL_ARRAY_BUFFER, allb_vertices.size() * sizeof(float),
               &allb_vertices[0], GL_STATIC_DRAW);
  // 设置顶点属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
}
void renderChunk()
{
  glDrawArrays(GL_TRIANGLES, 0, allb_vertices.size() * sizeof(float));
}
GLuint getworldVAO() { return worldVAO; }
