#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "headerLists.hpp"
struct Camera
{
  int width = 600, height = 400;
  bool mouseGet = true;
  bool keyFirst = true;
  glm::vec3 pos = glm::vec3(0.1f, 2.0f, 2.0f);
  glm::vec3 target = glm::vec3(0.1f, 2.0f, 0.0f);
  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 CF = glm::normalize(glm::vec3(target - pos));
  glm::vec3 CR = glm::normalize(glm::cross(CF, worldUp));
  glm::vec3 CU = glm::cross(CR, CF);
  GLfloat angleStep = 0.1f;
  GLfloat deltaTime = 0.0f;
  GLfloat lastframe = 0.0f;
  GLboolean mouse_is_first = true;
  GLdouble lastX = width / 2, lastY = height / 2;
  GLfloat Cyaw = -90.0f, Cpitch = 0.0f;
  GLfloat fov = 45.0f;
  glm::mat4 view = glm::mat4(1.0f), projection = glm::mat4(1.0f);
  Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp);
  static Camera &getCamera()
  {
    static Camera instance;
    return instance;
  }
  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;
  glm::mat4 viewMatrix();
  glm::mat4 perMatrix();
  void updateMatrix();
  void updateViewPort();
  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);

  static void reshapeCallback(GLFWwindow *window, int newwidth,
                              int newheight); // 窗口大小改变时执行
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods); // 按键按下时执行，适合快捷键操作
  void
  keyCallbackLongTime(GLFWwindow *window); // 持续监听键盘，适合连续进行的动作
  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void scrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset); // 鼠标滚轮滚动时执行
private:
  Camera() {}
};
#endif