#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL2/stb_image.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include"camera.hpp"
#include"load.hpp"

#define VAOS 2
GLuint vao[VAOS];
#define TEXS 1
GLuint tex[TEXS];
std::vector<GLuint> ebo1 = {2, 1, 0,
				3, 2, 0};

void init(GLFWwindow *window)
{
	cubeX = 0.0f;
	cubeY = 0.0f;
	cubeZ = -1.0f;
	glfwGetFramebufferSize(window, &width, &height);
}

void display(GLFWwindow *window, double currentTime, GLuint program,objLoader obj)
{
	glClearColor(0.3, 0.5, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(modelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, value_ptr(viewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(program, "per"), 1, GL_FALSE, value_ptr(perMatrix()));
	glUseProgram(program);
	glLineWidth(6);
	glBindVertexArray(vao[0]);
	glUniform1ui(glGetUniformLocation(program, "hasTex"), 0);
	glDrawArrays(GL_LINE_STRIP, 0, 5);

	glUseProgram(program);
	glBindVertexArray(obj.VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glUniform1ui(glGetUniformLocation(program, "hasTex"), 1);
	glUniform1i(glGetUniformLocation(program, "tex0"), 0);
	glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0);
}
int main()
{
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(800, 600, "demo", NULL, NULL);
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	GLuint program = fileloader("shaders/vmain.glsl", "shaders/fmain.glsl");
	//vao[0] = getPoints("../resources/models/cube.txt", false,ebo1);
	//vao[1] = getPoints("../resources/models/mod.txt", true, ebo1);

	tex[0] = loadTexture("../resources/imgs/tt.jpg");
	objLoader obj("../resources/models/mods.obj");

	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastframe;
		lastframe = glfwGetTime();
		keyCallbackLongTime(window);

		display(window, glfwGetTime(), program,obj);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
