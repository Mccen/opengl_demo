#include "Controler.hpp"
void Controler::getMouseMoveEvent(GLFWwindow *window, double xpos, double ypos)
{
  if (getControler().mouse_is_first)
  {
    getControler().lastX = xpos;
    getControler().lastY = ypos;
    getControler().mouse_is_first = false;
  }
  float xoffset = static_cast<float>(xpos - getControler().lastX);
  float yoffset = static_cast<float>(getControler().lastY - ypos);
  getControler().lastX = xpos;
  getControler().lastY = ypos;
  getControler().updateCamera(xoffset, yoffset);
}
void Controler::getMouseWheelEvent(GLFWwindow *window, double xoffset, double yoffset)
{
  Camera::getCamera().fov -= yoffset * getControler().mouseWheelSpeed;
  if (Camera::getCamera().fov < 30.0f)
  {
    Camera::getCamera().fov = 30.0f;
  }
  if (Camera::getCamera().fov > 60.0f)
  {
    Camera::getCamera().fov = 60.0f;
  }
}
void Controler::updateCamera(float yaw, float pitch)
{
  Camera::getCamera().c_yaw += yaw * mouseSpeed;
  Camera::getCamera().c_pitch += pitch * mouseSpeed;
  if (Camera::getCamera().c_pitch > 89.0f)
  {
    Camera::getCamera().c_pitch = 89.0f;
  }
  if (Camera::getCamera().c_pitch < -89.0f)
  {
    Camera::getCamera().c_pitch = -89.0f;
  }
  Camera::getCamera().CF = glm::normalize(glm::vec3(
      cos(glm::radians(Camera::getCamera().c_yaw)) * cos(glm::radians(Camera::getCamera().c_pitch)),
      sin(glm::radians(Camera::getCamera().c_pitch)),
      sin(glm::radians(Camera::getCamera().c_yaw)) * cos(glm::radians(Camera::getCamera().c_pitch))));
  Camera::getCamera().CR = glm::normalize(glm::cross(Camera::getCamera().CF, Camera::getCamera().e_up));
  Camera::getCamera().CU = glm::cross(Camera::getCamera().CR, Camera::getCamera().CF);
}
void Controler::getKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) &&
      (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && action == GLFW_PRESS)
  {
    if (getControler().mouseGet)
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetCursorPosCallback(window, nullptr);
      getControler().mouseGet = false;
    }
    else
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      getControler().mouse_is_first = true;
      glfwSetCursorPosCallback(window, getMouseMoveEvent);
      getControler().mouseGet = true;
    }
  }
  else if (action == GLFW_RELEASE)
  {
    getControler().keyFirst = true;
  }
}
void Controler::getKeyEventLongTime(GLFWwindow *window)
{
  GLfloat speed = 3.5f * getControler().deltaTime;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    speed *= 3.0f;
  }

  // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
  //   Camera::getCamera().e_position += speed * Camera::getCamera().CF;
  // }
  // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
  //   Camera::getCamera().e_position -= speed * Camera::getCamera().CF;
  // }
  // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
  //   Camera::getCamera().e_position -= speed * Camera::getCamera().CR;
  // }
  // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
  //   Camera::getCamera().e_position += speed * Camera::getCamera().CR;
  // }
  // if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
  //   Camera::getCamera().e_position -= speed * Camera::getCamera().CU;
  // }
  // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
  //   Camera::getCamera().e_position += speed * Camera::getCamera().CU;
  // }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    Camera::getCamera().e_position += speed * glm::normalize(glm::vec3(Camera::getCamera().CF.x, 0.0f, Camera::getCamera().CF.z));
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    Camera::getCamera().e_position -= speed * glm::normalize(glm::vec3(Camera::getCamera().CF.x, 0.0f, Camera::getCamera().CF.z));
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    Camera::getCamera().e_position -= speed * Camera::getCamera().CR;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    Camera::getCamera().e_position += speed * Camera::getCamera().CR;
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
  {
    Camera::getCamera().e_position -= speed * glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    Camera::getCamera().e_position += speed * glm::vec3(0.0f, 1.0f, 0.0f);
  }
  getControler().updateCameraMatrix();
}
void Controler::updateCameraMatrix()
{
  Camera::getCamera().getMVP();
}
void Controler::updateTime()
{
  float currentTime = glfwGetTime();
  deltaTime = currentTime - lastTime;
  lastTime = currentTime;
}