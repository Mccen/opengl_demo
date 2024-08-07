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

//使用枚举提高可读性
enum {
	Cube,
	Sphere
};
void init(GLFWwindow *window)
{
	cubeX = 0.0f;
	cubeY = 0.0f;
	cubeZ = -2.0f;
	modelMatrix();
	Light(light[0], glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	Material(material[0], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f);
	glfwGetFramebufferSize(window, &width, &height);
}

void display()
{
	float currentTime = glfwGetTime();
	glClearColor(0.3, 0.5, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 lmodel = glm::mat4(1.0f);
	lmodel = rotate(lmodel, glm::radians(currentTime)*20, glm::vec3(0.0f, 1.0f, 0.0f));
	lmodel = translate(lmodel, glm::vec3(2.0f,2.0f,2.0f));
	lmodel = scale(lmodel, glm::vec3(0.2f, 0.2f, 0.2f));
	glUseProgram(program[0]);
	glUniformMatrix4fv(glGetUniformLocation(program[0], "model"), 1, GL_FALSE, value_ptr(model*lmodel));
	glUniformMatrix4fv(glGetUniformLocation(program[0], "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[0], "projection"), 1, GL_FALSE, value_ptr(projection));
	glBindVertexArray(obj[Cube].VAO);
	glUniform1ui(glGetUniformLocation(program[0], "CC"), 2);
	glUniform1i(glGetUniformLocation(program[0], "fTex"), 0);
	glDrawElements(GL_TRIANGLES, obj[Cube].indices.size(), GL_UNSIGNED_INT, 0);


	glUseProgram(program[1]);
	glBindVertexArray(obj[Sphere].VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	light->position = (lmodel*glm::vec4(light->position,1.0f));
	light->updateUniform(program[1]);
	material->updateUniform(program[1]);
	glUniformMatrix4fv(glGetUniformLocation(program[1], "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program[1], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[1], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glDrawElements(GL_TRIANGLES, obj[Sphere].indices.size(), GL_UNSIGNED_INT, 0);


	glDisable(GL_DEPTH_TEST);
	glUseProgram(program[0]);
	glUniformMatrix4fv(glGetUniformLocation(program[0], "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program[0], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[0], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glLineWidth(6);
	glBindVertexArray(vao[0]);
	glUniform1ui(glGetUniformLocation(program[0], "CC"), 0);
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

	fileloader(program[0],"shaders/mainv.glsl", "shaders/mainf.glsl");
	fileloader(program[1],"shaders/lightv.glsl", "shaders/lightf.glsl");

	vao[0] = getPoints("../resources/models/cube.txt", false);

	loadTexture(tex[0],"../resources/imgs/earth.jpg");
	objLoader(obj[Cube],"../resources/models/block.obj");
	objLoader(obj[Sphere],"../resources/models/sphere.obj");
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastframe;
		lastframe = glfwGetTime();
		keyCallbackLongTime(window);
		updateUniform();
		display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
