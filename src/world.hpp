#ifndef WORLD_HPP
#define WORLD_HPP
#include "headerLists.hpp"
#define viewDistance 3
#define memorySize 32
template<class C>
struct worldVector{
  worldVector(C c)
  {

  }
  worldVector(C c, int n) {

  }
};
struct blockFaces
{
  // 下面
  GLfloat bottom[32];
  // 上面
  GLfloat top[32];
  // 前面
  GLfloat front[32];
  // 后面
  GLfloat back[32];
  // 左面
  GLfloat left[32];
  // 右面
  GLfloat right[32];
};
struct block
{
  bool isShow;
  glm::vec3 b_position;
  type b_type;
  void GenerateBlockVertices(blockFaces *blockFaces);
  block() {}
  block(glm::vec3 Pos, type type);
};
struct chunk
{
  // chunk的位置
  glm::vec2 c_position;
  // 区块边长
  int c_size = 16;
  int c_height = 128;
  block c_blocks[16][16][128];
  chunk() {}
  chunk(glm::vec2 position);
  void createBlock();
  bool hasBlock = false;
};
struct World
{
  GLuint worldVAO;
  glm::ivec2 cameraChunk;
  int *mapX, *mapZ;
  std::vector<std::vector<chunk,chunk[10]>,chunk[10]> memoryChunks;
  chunk viewChunks[(viewDistance * 2 + 1) * (viewDistance * 2 + 1)];
  GLfloat *vertices;
  bool isFirst = true;
  // 更新相机的chunk
  static void updateCameraChunk(glm::vec3 viewPosition);
  bool checkhasChunk(glm::ivec2 position);
  void createChunk(glm::ivec2 position);
  // 将坐标转换为chunks的索引
  glm::ivec2 changeCoordToIndex(glm::vec2 position);
  void makeVAO();
  void renderChunk();
  void loadViewChunk();
  void saveChunk();
  World &operator=(const World &) = delete;
  World(const World &) = delete;
  static World& getWorld(){
    static World m_world; return m_world;
  }

private:
  World();
};

#endif
