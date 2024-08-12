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

#include "camera.hpp"
#include "load.hpp"
#include "light.hpp"
#include "shadow.hpp"

#define VAOS 2
static GLuint vao[VAOS];
#define PROGRAMS 2
static GLuint program[PROGRAMS];
#define TEXTURES 2
static GLuint tex[TEXTURES];
#define OBJECTS 2
static objLoader obj[OBJECTS];
#define LIGHTS 1
static Light light[LIGHTS];
#define MATERIALS 1
static Material material[MATERIALS];

// 使用枚举提高可读性
// 模型
enum
{
	Cube,
	Sphere
};
// 着色器程序
enum
{
	lightProgram,
	mainProgram
};
void init(GLFWwindow *window)
{
	cubeX = 0.0f;
	cubeY = 0.0f;
	cubeZ = -2.0f;
	modelMatrix();
	fileloader(program[lightProgram], "resources/shaders/lightv.glsl", "resources/shaders/lightf.glsl");
	fileloader(program[mainProgram], "resources/shaders/mainv.glsl", "resources/shaders/mainf.glsl");
	vao[0] = getPoints("resources/models/cube.txt", false);
	loadTexture(tex[0], "resources/imgs/earth.jpg");
	objLoader(obj[Cube], "resources/models/block.obj");
	objLoader(obj[Sphere], "resources/models/sphere.obj");
	Light(light[0], glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(3.0f, 0.0f, 3.0f));
	Material(material[0], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 25.0f);
	updateViewPort();
	glfwGetFramebufferSize(window, &width, &height);
}

void display()
{
	float currentTime = glfwGetTime();
	glClearColor(0.3, 0.5, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 lmodel(1.0f);
	lmodel = glm::rotate(lmodel, glm::radians(currentTime) * 30, glm::vec3(0.0f, 1.0f, 0.0f));
	lmodel = glm::translate(lmodel, light[0].position);
	lmodel = glm::scale(lmodel, glm::vec3(0.2f, 0.2f, 0.2f));
	lmodel = model * lmodel;
	glUseProgram(program[lightProgram]);
	glBindVertexArray(obj[Cube].VAO);
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "model"), 1, GL_FALSE, value_ptr(lmodel));
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniform1ui(glGetUniformLocation(program[lightProgram], "CC"), 2);
	glUniform1i(glGetUniformLocation(program[lightProgram], "fTex"), 0);
	glDrawElements(GL_TRIANGLES, obj[Cube].indices.size(), GL_UNSIGNED_INT, 0);

	glUseProgram(program[mainProgram]);
	glBindVertexArray(obj[Sphere].VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	light->updateUniform(program[mainProgram]);
	material->updateUniform(program[mainProgram]);
	glUniformMatrix4fv(glGetUniformLocation(program[mainProgram], "lmodel"), 1, GL_FALSE, glm::value_ptr(lmodel));
	glUniformMatrix4fv(glGetUniformLocation(program[mainProgram], "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program[mainProgram], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[mainProgram], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glDrawElements(GL_TRIANGLES, obj[Sphere].indices.size(), GL_UNSIGNED_INT, 0);

	glDisable(GL_DEPTH_TEST);
	glUseProgram(program[lightProgram]);
	glBindVertexArray(vao[0]);
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[lightProgram], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glLineWidth(6);
	glUniform1ui(glGetUniformLocation(program[lightProgram], "CC"), 0);
	glDrawArrays(GL_LINE_STRIP, 0, 5);
}
int main()
{
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	// glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastframe;
		lastframe = glfwGetTime();
		keyCallbackLongTime(window);
		updateMatrix();
		updateViewPort();
		display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
