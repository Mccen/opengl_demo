#include "modules/Window.hpp"
#include "modules/Controler.hpp"
#include "modules/Camera.hpp"
#include "light.hpp"
#include "load.hpp"
#include "world.hpp"

#define VAOS 2
static GLuint vao[VAOS];
#define PROGRAMS 3
static GLuint program[PROGRAMS];
#define OBJECTS 2
static objLoader obj[OBJECTS];
#define LIGHTS 1
static Light light[LIGHTS];
#define MATERIALS 1
static Material material[MATERIALS];
#define TEXTURES 2
static GLuint tex[TEXTURES];

// 单例
//struct World &mworld = World::getWorld();
struct Controler &mcontroler = Controler::getControler();
struct Window &mwindow = Window::getWindow();
struct Camera &mcamera = Camera::getCamera();
void init(GLFWwindow *window)
{

#include "Lists.hpp"
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSwapInterval(1);
  glfwGetFramebufferSize(window, &mwindow.width, &mwindow.height);
  glfwSetWindowSizeCallback(window, mwindow.reshapeCallback);
  glfwSetCursorPosCallback(window, mcontroler.getMouseMoveEvent);
  glfwSetMouseButtonCallback(window, mcontroler.getMouseButtonEvent);
  glfwSetScrollCallback(window, mcontroler.getMouseWheelEvent);
  glfwSetKeyCallback(window, mcontroler.getKeyEvent);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLuint frames = 0;
GLfloat elapsedTime = 0.0f;
void fps(GLFWwindow *window)
{
  frames++;
  elapsedTime += mcontroler.deltaTime;
  if (elapsedTime >= 1.0f)
  {
    std::stringstream ss;
    ss << "FPS: " << frames <<"   "<< "pos:" << mcamera.e_position.x << "," <<mcamera.e_position.y << "," << mcamera.e_position.z;
    glfwSetWindowTitle(window, ss.str().c_str());
    frames = 0;
    elapsedTime = 0.0f;
  }
}
void display()
{
  glClearColor(0.3, 0.5, 0.4, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 lmodel(1.0f);
  lmodel = glm::scale(lmodel, glm::vec3(0.5f, 0.5f, 0.5f));
  glUseProgram(program[lightProgram]);
  glBindVertexArray(obj[Cube].VAO);
  glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "lmodel"), 1,
                     GL_FALSE, value_ptr(lmodel));
  glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "view"), 1,
                     GL_FALSE, value_ptr(mcamera.view));
  glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "projection"),
                     1, GL_FALSE, value_ptr(mcamera.projection));
  glUniform1i(glGetUniformLocation(program[lightProgram], "CC"), 2);
  glDrawElements(GL_TRIANGLES, obj[Cube].indices.size(), GL_UNSIGNED_INT, 0);

  // glUseProgram(program[worldProgram]);
  // glBindVertexArray();
  // glActiveTexture(GL_TEXTURE1);
  // glBindTexture(GL_TEXTURE_2D, tex[stone]);
  // glUniform1i(glGetUniformLocation(program[worldProgram], "fTex"), 1);
  // light->updateUniform(program[worldProgram]);
  // material->updateUniform(program[worldProgram]);
  // glm::mat4 smodel =
  //     glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  // glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "lmodel"), 1,
  //                    GL_FALSE, glm::value_ptr(lmodel));
  // glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "model"), 1,
  //                    GL_FALSE, glm::value_ptr(smodel));
  // glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "view"), 1,
  //                    GL_FALSE, glm::value_ptr(camera.view));
  // glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "projection"),
  //                    1, GL_FALSE, glm::value_ptr(camera.projection));
}
int main()
{
  mwindow.createWindow(800, 600, "OpenGL", 3, 3);
  GLFWwindow *window = mwindow.window;

  init(window);
  mcamera.createCamera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
  while (!glfwWindowShouldClose(window))
  {
    mcontroler.updateTime();
    mcontroler.getKeyEventLongTime(window);
    fps(window);
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
