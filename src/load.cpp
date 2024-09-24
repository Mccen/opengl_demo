#include "load.hpp"
#include <fstream>
objLoader::objLoader() {}
objLoader::objLoader(const char *addr) {
  std::vector<GLfloat> tempVN, tempV, tempVT;
  std::vector<GLuint> tempVInd, tempVTInd, tempVNInd;
  std::ifstream file(addr);
  if (!file.is_open()) {
    std::cout << "\033[31mfile open error\033[m" << std::endl;
    throw std::runtime_error("file open error");
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue; // 跳过空行和注释行
    std::istringstream iss(line);
    std::string type;
    iss >> type;
    if (type == "vn") {
      readVector(iss, tempVN);
    } else if (type == "v") {
      readVector(iss, tempV);
    } else if (type == "vt") {
      readVector(iss, tempVT);
    } else if (type == "f") {
      readFace(iss, tempVInd, tempVTInd, tempVNInd);
    }
  }

  int num = tempVInd.size();
  for (int i = 0; i < num; ++i) {
    Vertex temp;
    temp.position[0] = tempV[tempVInd[i] * 3 + 0];
    temp.position[1] = tempV[tempVInd[i] * 3 + 1];
    temp.position[2] = tempV[tempVInd[i] * 3 + 2];

    temp.texCoord[0] = tempVT[tempVTInd[i] * 2 + 0];
    temp.texCoord[1] = tempVT[tempVTInd[i] * 2 + 1];

    temp.normal[0] = tempVN[tempVNInd[i] * 3 + 0];
    temp.normal[1] = tempVN[tempVNInd[i] * 3 + 1];
    temp.normal[2] = tempVN[tempVNInd[i] * 3 + 2];

    vertices.push_back(temp);
    indices.push_back(i);
  }
  setupMesh();
  tempV.clear();
  tempVN.clear();
  tempVT.clear();
  tempVTInd.clear();
  tempVNInd.clear();
  tempVInd.clear();
}

void objLoader::readVector(std::istringstream &iss,
                           std::vector<GLfloat> &data) {
  std::string token;
  GLfloat temp;
  while (iss >> token) {
    temp = std::stof(token);
    data.push_back(temp);
  }
}

void objLoader::readFace(std::istringstream &iss, std::vector<GLuint> &vInd,
                         std::vector<GLuint> &vtInd,
                         std::vector<GLuint> &vnInd) {
  std::string tokens;
  while (iss >> tokens) {
    std::stringstream token(tokens);
    std::string cell;
    int ii = 0;
    int ind[3] = {0, 0, 0};
    while (std::getline(token, cell, '/')) {
      if (!cell.empty()) {
        ind[ii] = std::stoi(cell) - 1;
      }
      ++ii;
    }
    vInd.push_back(ind[0]);
    vtInd.push_back(ind[1]);
    vnInd.push_back(ind[2]);
  }
}

void objLoader::setupMesh() {
  glGenVertexArrays(1, &(this->VAO));
  glGenBuffers(1, &(this->VBO));
  glGenBuffers(1, &(this->EBO));

  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, position));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoord));

  glBindVertexArray(0);
}
void errorGet(GLuint shader) {
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    GLsizei infoLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    GLchar *info = new GLchar(infoLen);
    if (info != NULL) {
      glGetShaderInfoLog(shader, 1024, NULL, info);
      std::cout << "\033[31merror:" << std::endl
                << info << "\033[m" << std::endl;
    }
  }
}

GLuint fileloader(const char *addrv, const char *addrf) {
  std::fstream filev(addrv, std::ios::in);
  std::fstream filef(addrf, std::ios::in);
  std::stringstream sv, sf;
  if (!filev.is_open()) {
    std::cout << "\033[31mvertex open faild\033[m";
    exit(-1);
  }
  sv << filev.rdbuf();
  if (!filef.is_open()) {
    std::cout << "\033[31mfragment open faild\033[m";
    exit(-1);
  }
  sf << filef.rdbuf();
  std::string strv = sv.str();
  std::string strf = sf.str();
  const char *vcode = strv.c_str();
  const char *fcode = strf.c_str();
  GLuint vshader = 0, fshader = 0;
  vshader = glCreateShader(GL_VERTEX_SHADER);
  fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vshader, 1, &vcode, NULL);
  glCompileShader(vshader);
  errorGet(vshader);
  glShaderSource(fshader, 1, &fcode, NULL);
  glCompileShader(fshader);
  errorGet(fshader);
  GLuint program = glCreateProgram();
  glAttachShader(program, vshader);
  glAttachShader(program, fshader);
  glLinkProgram(program);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  return program;
}

GLuint getPoints(const char *addr, bool enableEBO) {
  std::ifstream file(addr, std::ios::in);
  if (!file.is_open()) {
    std::cout << "\033[31mfile open faild\033[m";
    exit(-1);
  }
  std::string str;
  std::vector<float> points;
  while (getline(file, str)) {
    std::stringstream sl(str);
    std::string cell;
    while (getline(sl, cell, ',')) {
      points.push_back(stof(cell));
    }
  }
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points.size(), points.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return VAO;
}

GLuint loadTexture(const char *addr) {
  GLuint texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(addr, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    GLfloat anisoSetting = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisoSetting);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisoSetting);
  } else {
    std::cout << "\033[31mtexture load error\033[m" << std::endl;
    exit(-1);
  }
  stbi_image_free(data);
  return texture;
}