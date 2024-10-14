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
      -sin(glm::radians(Camera::getCamera().c_yaw)) * cos(glm::radians(Camera::getCamera().c_pitch)),
      sin(glm::radians(Camera::getCamera().c_pitch)),
      cos(glm::radians(Camera::getCamera().c_yaw)) * cos(glm::radians(Camera::getCamera().c_pitch))));
  Camera::getCamera().CR = glm::normalize(glm::cross(Camera::getCamera().e_up, Camera::getCamera().CF));
  Camera::getCamera().CU = glm::cross(Camera::getCamera().CF, Camera::getCamera().CR);
}
void Controler::getKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  // 获取Controler对象的引用
  Controler &controler = getControler();

  // 获取键的状态
  bool escapePressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
  bool f11Pressed = glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS;
  bool ctrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
  bool mPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;

  switch (action)
  {
  case GLFW_PRESS:
    if (escapePressed)
    {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (!controler.isKeyActive)
    {
      if (f11Pressed)
      {
        controler.setFullScreen(window);
        controler.isKeyActive = true;
      }
      if (ctrlPressed && mPressed)
      {
        if (controler.mouseGet)
        {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
          glfwSetCursorPosCallback(window, nullptr);
        }
        else
        {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          glfwSetCursorPosCallback(window, getMouseMoveEvent);
          controler.mouse_is_first = true;
        }
        controler.mouseGet = !controler.mouseGet;
        controler.isKeyActive = true;
      }
    }
    break;
  case GLFW_RELEASE:
    controler.isKeyActive = false;
    break;
  }
}
void Controler::getKeyEventLongTime(GLFWwindow *window)
{
  GLfloat speed = 3.5f * getControler().deltaTime;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    speed *= 3.0f;
  }

  Camera &camera = Camera::getCamera(); // 获取相机对象的引用

  // 获取键的状态
  bool wPressed = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
  bool sPressed = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
  bool aPressed = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
  bool dPressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
  bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
  bool spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

  // 更新相机位置
  if (wPressed)
  {
    camera.e_position += speed * glm::normalize(glm::vec3(camera.CF.x, 0.0f, camera.CF.z));
  }
  if (sPressed)
  {
    camera.e_position -= speed * glm::normalize(glm::vec3(camera.CF.x, 0.0f, camera.CF.z));
  }
  if (aPressed)
  {
    camera.e_position += speed * camera.CR;
  }
  if (dPressed)
  {
    camera.e_position -= speed * camera.CR;
  }
  if (cPressed || spacePressed) // 合并上下移动的条件判断
  {
    camera.e_position.y += (spacePressed ? 1.0f : -1.0f) * speed;
  }

  getControler().updateCameraMatrix(); // 更新相机矩阵
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
void Controler::setFullScreen(GLFWwindow *window)
{
  // 获取对象的引用
  Controler &controler = getControler();
  Camera &camera = Camera::getCamera();

  // 获取主显示器的指针和视频模式
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  if (!monitor)
  {
    // 处理错误，例如打印日志或抛出异常
    std::cout << "Failed to get primary monitor" << std::endl;
    return;
  }
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  if (!mode)
  {
    // 处理错误，例如打印日志或抛出异常
    std::cout << "Failed to get video mode for primary monitor" << std::endl;
    return;
  }

  // 切换全屏和窗口模式
  if (controler.isFullScreen)
  {
    // 切换回窗口模式，使用配置的位置和大小
    glfwSetWindowMonitor(window, NULL, camera.windowPosX, camera.windowPosY, camera.windowWidth, camera.windowHeight, GLFW_DONT_CARE);
    controler.isFullScreen = false;
  }
  else
  {
    // 获取当前窗口的位置和大小
    glfwGetWindowPos(window, &camera.windowPosX, &camera.windowPosY);
    glfwGetWindowSize(window, &camera.windowWidth, &camera.windowHeight);
    // 切换到全屏模式，使用主显示器的分辨率和刷新率
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    controler.isFullScreen = true;
  }
}