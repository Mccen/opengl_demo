#include "camera.hpp"
Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp) {
  this->pos = pos;
  this->target = target;
  this->worldUp = worldUp;
  CF = glm::normalize(glm::vec3(target - pos));
  CR = glm::normalize(glm::cross(CF, worldUp));
  CU = glm::cross(CR, CF);
}
glm::mat4 Camera::viewMatrix() {
  view = glm::lookAt(pos, pos + CF, CU);
  return view;
}
glm::mat4 Camera::perMatrix() {
  GLfloat aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
  GLfloat near = 0.1f;
  GLfloat far = 100.0f;
  projection = glm::perspective(glm::radians(fov), aspect, near, far);
  return projection;
}
void Camera::updateMatrix() {
  viewMatrix();
  perMatrix();
}
void Camera::updateViewPort() {}
void Camera::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  if (getCamera().mouse_is_first) {
    getCamera().lastX = xpos;
    getCamera().lastY = ypos;
    getCamera().mouse_is_first = false;
  }
  float xoffset = static_cast<float>(xpos - getCamera().lastX);
  float yoffset = static_cast<float>(getCamera().lastY - ypos);
  getCamera().lastX = xpos;
  getCamera().lastY = ypos;
  getCamera().Cyaw += xoffset * 0.1f;
  getCamera().Cpitch += yoffset * 0.1f;
  if (getCamera().Cpitch > 89.0f) {
    getCamera().Cpitch = 89.0f;
  }
  if (getCamera().Cpitch < -89.0f) {
    getCamera().Cpitch = -89.0f;
  }
  getCamera().CF = glm::normalize(glm::vec3(
      cos(glm::radians(getCamera().Cyaw)) *
          cos(glm::radians(getCamera().Cpitch)),
      sin(glm::radians(getCamera().Cpitch)),
      sin(glm::radians(getCamera().Cyaw)) *
          cos(glm::radians(getCamera().Cpitch))));
  getCamera().CR = glm::normalize(glm::cross(getCamera().CF, getCamera().worldUp));
  getCamera().CU = glm::cross(getCamera().CR, getCamera().CF);
}

void Camera::closeCallback(GLFWwindow *window) // 窗口关闭时执行
{
  std::cout << "window close" << std::endl;
}
void Camera::reshapeCallback(GLFWwindow *window, int newwidth,
                             int newheight) // 窗口大小改变时执行
{
  getCamera().width = newwidth;
  getCamera().height = newheight;
  glViewport(0, 0, getCamera().width, getCamera().height);
}
void Camera::keyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) // 按键按下时执行，适合快捷键操作
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) &&
      (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && action == GLFW_PRESS) {
    if (getCamera().mouseGet) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetCursorPosCallback(window, nullptr);
      getCamera().mouseGet = false;
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      getCamera().mouse_is_first = true;

      glfwSetCursorPosCallback(window, mouseCallback);
      getCamera().mouseGet = true;
    }
  } else if (action == GLFW_RELEASE) {
    getCamera().keyFirst = true;
  }
}
void Camera::keyCallbackLongTime(
    GLFWwindow *window) // 持续监听键盘，适合连续进行的动作
{
  GLfloat speed = 3.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speed *= 4.0f;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    pos += speed * CF;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    pos -= speed * CF;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    pos -= speed * CR;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    pos += speed * CR;
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    pos -= speed * CU;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    pos += speed * CU;
  }
  updateMatrix();
}
void Camera::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    getCamera().mouse_is_first = true;
  }
}
void Camera::scrollCallback(GLFWwindow *window, double xoffset,
                            double yoffset) // 鼠标滚轮滚动时执行
{
  getCamera().fov -= yoffset;
  if (getCamera().fov < 30.0f) {
    getCamera().fov = 30.0f;
  }
  if (getCamera().fov > 60.0f) {
    getCamera().fov = 60.0f;
  }
}
