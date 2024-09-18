// #include <cstddef>
#include "headerLists.hpp"

struct Vertex {
  GLfloat position[3];
  GLfloat normal[3];
  GLfloat texCoord[2];
};

class objLoader {
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

GLuint fileloader(const char *addrv, const char *addrf);

GLuint getPoints(const char *addr, bool enableEBO);
GLuint loadTexture(const char *addr);