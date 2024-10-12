#ifndef WORLD_HPP
#define WORLD_HPP
#define viewDistance 3
#define memorySize 32

#include "headerLists.hpp"
#include "utils/Vector.hpp"
#include "utils/Noise.hpp"
#include "utils/Mesh.hpp"
#include "utils/Single.hpp"
struct block
{
  enum face
  {
    bottom = 0,
    top,
    left,
    right,
    front,
    back
  };
  bool isShow;
  glm::vec3 b_position;
  type b_type;
  Vector<Mesh, 3> b_mesh;
  void GenBottomVertices();
  void GenTopVertices();
  void GenLeftVertices();
  void GenRightVertices();
  void GenFrontVertices();
  void GenBackVertices();
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
struct World : Single

{
  GLuint worldVAO;
  glm::ivec2 cameraChunk;
  int *mapX, *mapZ;
  Vector<Vector<chunk, 5>, 5> memoryChunks;
  chunk viewChunks[(viewDistance * 2 + 1) * (viewDistance * 2 + 1)];
  GLfloat *vertices;
  bool isFirst = true;
  // 更新相机的chunk
  static void updateCameraChunk(glm::vec3 viewPosition);
  // 检查是否有chunk
  bool checkhasChunk(glm::ivec2 position);
  // 创建chunk
  void createChunk(glm::ivec2 position);
  // 将坐标转换为chunks的索引
  glm::ivec2 changeCoordToIndex(glm::vec2 position);
  void makeVAO();
  void renderChunk();
  void loadViewChunk();
  void saveChunk();

  static World &getWorld()
  {
    static World m_world;
    return m_world;
  }

private:
  World();
};

#endif
