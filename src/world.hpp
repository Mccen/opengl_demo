#include "headerLists.hpp"
// Perlin噪声函数
int perlinNoise(float x, float y);

struct block
{
  glm::vec3 b_position;
  type b_type;
  std::vector<GLfloat> b_vertices;
  block(glm::vec3 Pos, type type);
};
struct chunk
{
  // chunk的位置
  glm::vec2 c_position;
  // 区块边长
  int c_size = 16;
  int c_height = 256;
  std::vector<block> c_blocks;
  chunk();
  chunk(glm::vec2 position);
  void createBlock();
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
void getViewChunk(glm::vec3 viewPosition);

void loadChunk();
std::vector<float> generateChunkvertices(const chunk &c);

void updateworldVAO(glm::vec3 viewPosition);
void renderChunk();
GLuint getworldVAO();