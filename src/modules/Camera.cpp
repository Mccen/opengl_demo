#include "Camera.hpp"
void Camera::createCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp)
{
  this->e_position = pos;
  this->e_target = target;
  this->e_up = worldUp;
  CF = glm::normalize(glm::vec3(target - pos));
  CR = glm::normalize(glm::cross(CF, worldUp));
  CU = glm::cross(CR, CF);
}
glm::mat4 Camera::viewMatrix()
{
  view = glm::lookAt(e_position, e_position + CF, CU);
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
void Camera::getVP()
{
  this->VP = perMatrix() * viewMatrix();
  // 左平面
  getCamera().planes[0].normal = glm::vec3(VP[0][3] + VP[0][0], VP[1][3] + VP[1][0], VP[2][3] + VP[2][0]);
  getCamera().planes[0].distance = VP[3][3] + VP[3][0];

  // 右平面
  getCamera().planes[1].normal = glm::vec3(VP[0][3] - VP[0][0], VP[1][3] - VP[1][0], VP[2][3] - VP[2][0]);
  getCamera().planes[1].distance = VP[3][3] - VP[3][0];

  // 下平面
  getCamera().planes[2].normal = glm::vec3(VP[0][3] + VP[0][1], VP[1][3] + VP[1][1], VP[2][3] + VP[2][1]);
  getCamera().planes[2].distance = VP[3][3] + VP[3][1];

  // 上平面
  getCamera().planes[3].normal = glm::vec3(VP[0][3] - VP[0][1], VP[1][3] - VP[1][1], VP[2][3] - VP[2][1]);
  getCamera().planes[3].distance = VP[3][3] - VP[3][1];

  // 近平面
  getCamera().planes[4].normal = glm::vec3(VP[0][2], VP[1][2], VP[2][2]);
  getCamera().planes[4].distance = VP[3][2];

  // 远平面
  getCamera().planes[5].normal = glm::vec3(-VP[0][2], -VP[1][2], -VP[2][2]);
  getCamera().planes[5].distance = -VP[3][2];

  // 归一化平面法向量
  for (auto &plane : getCamera().planes)
  {
    float length = glm::length(plane.normal);
    plane.normal /= length;
    plane.distance /= length;
  }
}
