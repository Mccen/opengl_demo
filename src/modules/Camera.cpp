#include "Camera.hpp"
void Camera::createCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp)
{
  this->e_position = pos;
  this->e_target = target;
  this->e_up = worldUp;
  //建立坐标系,以相机为基准
  //由pos指向target
  CF = glm::normalize(glm::vec3(target - pos));
  //右向量
  CR = glm::normalize(glm::cross(worldUp, CF));
  //垂直向量
  CU = glm::cross(CF,CR);
}
glm::mat4 Camera::modelMatrix(){
  model = glm::translate(glm::mat4(1.0f), e_position);
  return model;
}
glm::mat4 Camera::viewMatrix()
{
  view = glm::lookAt(e_position, e_position + CF, CU);
  view = glm::scale(view, glm::vec3(0.5f, 0.5f, 0.5f));
  return view;
}
glm::mat4 Camera::perMatrix()
{
  GLfloat aspect = static_cast<GLfloat>(Window::width) / static_cast<GLfloat>(Window::height);
  GLfloat near = 0.1f;
  GLfloat far = 100.0f;
  projection = glm::perspective(glm::radians(fov), aspect, near, far);
  return projection;
}
void Camera::getMVP()
{
  MVP = perMatrix() * viewMatrix()* modelMatrix();
  // 左平面
  getCamera().planes[0].normal = glm::vec3(MVP[0][3] + MVP[0][0], MVP[1][3] + MVP[1][0], MVP[2][3] + MVP[2][0]);
  getCamera().planes[0].distance = MVP[3][3] + MVP[3][0];

  // 右平面
  getCamera().planes[1].normal = glm::vec3(MVP[0][3] - MVP[0][0], MVP[1][3] - MVP[1][0], MVP[2][3] - MVP[2][0]);
  getCamera().planes[1].distance = MVP[3][3] - MVP[3][0];

  // 下平面
  getCamera().planes[2].normal = glm::vec3(MVP[0][3] + MVP[0][1], MVP[1][3] + MVP[1][1], MVP[2][3] + MVP[2][1]);
  getCamera().planes[2].distance = MVP[3][3] + MVP[3][1];

  // 上平面
  getCamera().planes[3].normal = glm::vec3(MVP[0][3] - MVP[0][1], MVP[1][3] - MVP[1][1], MVP[2][3] - MVP[2][1]);
  getCamera().planes[3].distance = MVP[3][3] - MVP[3][1];

  // 近平面
  getCamera().planes[4].normal = glm::vec3(MVP[0][2], MVP[1][2], MVP[2][2]);
  getCamera().planes[4].distance = MVP[3][2];

  // 远平面
  getCamera().planes[5].normal = glm::vec3(-MVP[0][2], -MVP[1][2], -MVP[2][2]);
  getCamera().planes[5].distance = -MVP[3][2];

  // 归一化平面法向量
  for (auto &plane : getCamera().planes)
  {
    float length = glm::length(plane.normal);
    plane.normal /= length;
    plane.distance /= length;
  }
}
