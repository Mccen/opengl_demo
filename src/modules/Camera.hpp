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
  /**
   * 需要特殊说明的是,由于传统opengl的右手坐标系不便于理解(是的,是我不理解)
   * 本相机系统建立的坐标系原点为面对屏幕的人,正面为z正,右面为x正,上面为y正
   * 相机默认视线与人同步,即朝向(0,0,1)
   * c_yaw的默认角度为相对z正的偏移角,左偏为负右偏为正
   * c_pitch的默认角度为相对z正的偏移角,上偏为正下偏为负
   */
struct Camera : Entity, Single
{

  //相机的前,右,上向量
  glm::vec3 CF,CR,CU;
  // 相机的偏航角和俯仰角
  GLfloat c_yaw = 0.0f, c_pitch = 0.0f;
  // 相机的视野范围
  GLfloat fov = 45.0f;
  // 临时储存窗口的位置及宽高
  int windowPosX, windowPosY, windowWidth, windowHeight;
  // 相机的模型矩阵,视图矩阵,投影矩阵
  glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
  // 相机的MVP矩阵
  glm::mat4 MVP;
  // 视锥体
  Plane planes[6];


  void createCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp);
  glm::mat4 modelMatrix();
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