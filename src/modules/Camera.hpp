#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "../headerLists.hpp"
#include "Window.hpp"
#include "Entity.hpp"
#include "../utils/Single.hpp"
struct Window;
struct Plane{
  glm::vec3 normal;
  float distance;
};
struct Camera : Entity, Single
{

  //相机的前,右,上向量
  glm::vec3 CF,CR,CU;
  // 相机的偏航角和俯仰角
  GLfloat c_yaw = -90.0f, c_pitch = 0.0f;
  // 相机的视野范围
  GLfloat fov = 45.0f;
  // 临时储存窗口的位置及宽高
  int windowPosX, windowPosY, windowWidth, windowHeight;
  // 相机的视图矩阵,投影矩阵
  glm::mat4  view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
  // 相机的VP矩阵
  glm::mat4 VP=glm::mat4(1.0f);
  // 视锥体
  Plane planes[6];


  void createCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp);
  glm::mat4 viewMatrix();
  glm::mat4 perMatrix();
  void getVP();
  static Camera &getCamera()
  {
    static Camera camera;
    return camera;
  }

private:
  Camera() = default;
};
#endif