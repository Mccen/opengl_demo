#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "../headerLists.hpp"
#include "Window.hpp"
#include "Entity.hpp"
#include "../utils/Single.hpp"
struct Window;
struct Camera : Entity, Single
{

  //相机的前,右,上向量
  glm::vec3 CF,CR,CU;
  //视锥体的六个面
  glm::vec4 planes[6];
  // 相机的偏航角和俯仰角
  GLfloat c_yaw = -90.0f, c_pitch = 0.0f;
  // 相机的视野范围
  GLfloat fov = 45.0f;

  glm::mat4 Model=glm::mat4(1.0f), view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
  glm::mat4 MVP=glm::mat4(1.0f);
  void createCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp);
  glm::mat4 viewMatrix();
  glm::mat4 perMatrix();

  void getMVP();
  static Camera &getCamera()
  {
    static Camera camera;
    return camera;
  }

private:
  Camera() = default;
};
#endif