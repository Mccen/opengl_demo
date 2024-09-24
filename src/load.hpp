#include "headerLists.hpp"
struct Vertex
{
  GLfloat position[3];
  GLfloat normal[3];
  GLfloat texCoord[2];
};

class objLoader
{
public:
  GLuint VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  objLoader();
  objLoader(const char *addr);

private:
  void readVector(std::istringstream &iss, std::vector<GLfloat> &data);

  void readFace(std::istringstream &iss, std::vector<GLuint> &vInd,
                std::vector<GLuint> &vtInd, std::vector<GLuint> &vnInd);

  void setupMesh();
};
void errorGet(GLuint shader);

GLuint fileloader(const char *addrv, const char *addrf); // 用于读取着色器代码
GLuint getPoints(const char *addr, bool enableEBO);      // 用于读取简易的顶点数据
GLuint loadTexture(const char *addr);                    // 用于读取纹理