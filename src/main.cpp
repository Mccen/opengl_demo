#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL2/stb_image.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "enum.hpp"
#include "camera.hpp"
#include "load.hpp"
#include "light.hpp"
#include "shadow.hpp"
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

void init(GLFWwindow *window)
{
#include "Lists.hpp"
	updateViewPort();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwGetFramebufferSize(window, &width, &height);
}

void fps(GLFWwindow *window)
{
	frames++;
	elapsedTime += deltaTime;
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
	lmodel = glm::rotate(lmodel, glm::radians(currentTime) * 30, glm::vec3(0.0f, 1.0f, 0.0f));
	lmodel = glm::translate(lmodel, light[0].position);
	lmodel = glm::scale(lmodel, glm::vec3(0.2f, 0.2f, 0.2f));
	glUseProgram(program[lightProgram]);
	glBindVertexArray(obj[Cube].VAO);
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "lmodel"), 1, GL_FALSE, value_ptr(lmodel));
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniform1ui(glGetUniformLocation(program[lightProgram], "CC"), 2);
	glDrawElements(GL_TRIANGLES, obj[Cube].indices.size(), GL_UNSIGNED_INT, 0);

	glUseProgram(program[worldProgram]);
	glBindVertexArray(worldVAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex[stone]);
	glUniform1i(glGetUniformLocation(program[worldProgram], "fTex"), 1);
	light->updateUniform(program[worldProgram]);
	material->updateUniform(program[worldProgram]);
	glm::mat4 smodel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "lmodel"), 1, GL_FALSE, glm::value_ptr(lmodel));
	glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "model"), 1, GL_FALSE, glm::value_ptr(smodel));
	glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[worldProgram], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	renderChunk();
}
int main()
{
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(width, height, "demo", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);
	init(window);
	glfwSetWindowCloseCallback(window, closeCallback);
	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastframe;
		lastframe = glfwGetTime();
		keyCallbackLongTime(window);
		updateworldVAO(pos);
		fps(window);
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
