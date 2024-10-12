#include "Window.hpp"
int Window::width = 0;
int Window::height = 0;
void Window::createWindow(unsigned int width, unsigned int height, const char *title, unsigned int openglMajor, unsigned int openglMinor)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  Window::width = width;
  Window::height = height;
  this->title = title;
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  glfwMakeContextCurrent(window);

}
void Window::reshapeCallback(GLFWwindow *window, int newwidth, int newheight) // 窗口大小改变时执行
{
  Window::width = newwidth;
  Window::height = newheight;
  glViewport(0, 0, Window::width, Window::height);
}