#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL2/stb_image.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "load.hpp"
#include "light.hpp"

#define VAOS 2
GLuint vao[VAOS];
#define PROS 2
GLuint program[PROS];
#define TEXS 1
GLuint tex[TEXS];
#define OBJS 2
objLoader obj[OBJS];

Light light[1];
Material material[1];

std::vector<GLuint> ebo1 = {2, 1, 0,
							3, 2, 0};
GLuint globalAmbLoc,ambLoc, diffLoc, specLoc, posLoc, dirLoc, attLoc, spotCutoffLoc, spotExponentLoc, spotDirLoc;
void init(GLFWwindow *window)
{
	cubeX = 0.0f;
	cubeY = 0.0f;
	cubeZ = -1.0f;
	light[0] = Light(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), vec3(2.0f,2.0f,2.0f));
	material[0] = Material(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f);
	glfwGetFramebufferSize(window, &width, &height);
}

void display(GLFWwindow *window)
{
	float currentTime = glfwGetTime();
	glClearColor(0.3, 0.5, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	mat4 lmodel = mat4(1.0f);
	lmodel = rotate(lmodel, radians(currentTime)*20, vec3(0.0f, 1.0f, 0.0f));
	lmodel = translate(lmodel, vec3(2.0f,2.0f,2.0f));
	lmodel = scale(lmodel, vec3(0.2f, 0.2f, 0.2f));
	glUseProgram(program[0]);
	glUniformMatrix4fv(glGetUniformLocation(program[0], "model"), 1, GL_FALSE, value_ptr(lmodel));
	glUniformMatrix4fv(glGetUniformLocation(program[0], "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[0], "projection"), 1, GL_FALSE, value_ptr(projection));
	glBindVertexArray(obj[1].VAO);
	glUniform1ui(glGetUniformLocation(program[0], "CC"), 2);
	glUniform1i(glGetUniformLocation(program[0], "fTex"), 0);
	glDrawElements(GL_TRIANGLES, obj[1].indices.size(), GL_UNSIGNED_INT, 0);


	glUseProgram(program[1]);
	glBindVertexArray(obj[0].VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	light->position = (lmodel*vec4(light->position,1.0f));
	light->updateUniform(program[1]);
	material->updateUniform(program[1]);
	glUniformMatrix4fv(glGetUniformLocation(program[1], "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program[1], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program[1], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glDrawElements(GL_TRIANGLES, obj[0].indices.size(), GL_UNSIGNED_INT, 0);


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
	program[0] = fileloader("shaders/mainv.glsl", "shaders/mainf.glsl");
	program[1] = fileloader("shaders/lightv.glsl", "shaders/lightf.glsl");

	vao[0] = getPoints("../resources/models/cube.txt", false, ebo1);

	tex[0] = loadTexture("../resources/imgs/earth.jpg");
	obj[0] = objLoader("../resources/models/sphere.obj");
	obj[1] = objLoader("../resources/models/block.obj");
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastframe;
		lastframe = glfwGetTime();
		keyCallbackLongTime(window);
		updateUniform();
		display(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
