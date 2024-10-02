#include "camera.hpp"
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
GLuint frames = 0;
GLfloat elapsedTime = 0.0f;

// 单例
Camera &camera = Camera::getCamera();
World &world = World::getWorld();
void init(GLFWwindow *window)
{

#include "Lists.hpp"
  camera.updateViewPort();
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSwapInterval(1);
  glfwGetFramebufferSize(window, &camera.width, &camera.height);
  glfwSetWindowSizeCallback(window, camera.reshapeCallback);
  glfwSetCursorPosCallback(window, camera.mouseCallback);
  glfwSetMouseButtonCallback(window, camera.mouseButtonCallback);
  glfwSetScrollCallback(window, camera.scrollCallback);
  glfwSetKeyCallback(window, camera.keyCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void fps(GLFWwindow *window)
{
  frames++;
  elapsedTime += camera.deltaTime;
  if (elapsedTime >= 1.0f)
  {
    std::stringstream ss;
    ss << "FPS: " << frames;
    glfwSetWindowTitle(window, ss.str().c_str());
    frames = 0;
    elapsedTime = 0.0f;
  }
}
void display()
{
  float currentTime = glfwGetTime();
  glClearColor(0.3, 0.5, 0.4, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 lmodel(1.0f);
  lmodel = glm::rotate(lmodel, glm::radians(currentTime) * 30,
                       glm::vec3(0.0f, 1.0f, 0.0f));
  lmodel = glm::translate(lmodel, light[0].position);
  lmodel = glm::scale(lmodel, glm::vec3(0.2f, 0.2f, 0.2f));
  glUseProgram(program[lightProgram]);
  glBindVertexArray(obj[Cube].VAO);
  glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "lmodel"), 1,
                     GL_FALSE, value_ptr(lmodel));
  glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "view"), 1,
                     GL_FALSE, value_ptr(camera.view));
  glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "projection"),
                     1, GL_FALSE, value_ptr(camera.projection));
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
  glfwInit();
  glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window =
  glfwCreateWindow(camera.width, camera.height, "demo", NULL, NULL);
  glfwMakeContextCurrent(window);
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0)
  {
    printf("Failed to initialize OpenGL context\n");
    return -1;
  }
  printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));
  init(window);
  while (!glfwWindowShouldClose(window))
  {
    camera.deltaTime = glfwGetTime() - camera.lastframe;
    camera.lastframe = glfwGetTime();
    camera.keyCallbackLongTime(window);
    world.updateCameraChunk(camera.pos);
    fps(window);
    display();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
