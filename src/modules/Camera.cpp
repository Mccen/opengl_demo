#include "Camera.hpp"
void Camera::createCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp) {
  this->e_position = pos;
  this->e_target = target;
  this->e_up = worldUp;
  CF = glm::normalize(glm::vec3(target - pos));
  CR = glm::normalize(glm::cross(CF, worldUp));
  CU = glm::cross(CR, CF);
}
glm::mat4 Camera::viewMatrix() {
  view = glm::lookAt(e_position, e_position + CF, CU);
  return view;
}
glm::mat4 Camera::perMatrix() {
  GLfloat aspect = static_cast<GLfloat>(Window::width) / static_cast<GLfloat>(Window::height);
  GLfloat near = 0.1f;
  GLfloat far = 100.0f;
  projection = glm::perspective(glm::radians(fov), aspect, near, far);
  return projection;
}
void Camera::getMVP() {
  this->MVP = perMatrix() * viewMatrix() * Model;
}
