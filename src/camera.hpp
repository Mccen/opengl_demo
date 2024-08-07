
int width = 600, height=400;
bool mouseGet = true;
bool keyFirst = true;
glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 CF = normalize(glm::vec3(target - pos));
glm::vec3 CR = normalize(cross(CF, worldUp));
glm::vec3 CU = cross(CR, CF);
GLfloat angleStep = 0.1f;
GLfloat deltaTime = 0.0f;
GLfloat lastframe = 0.0f;
GLfloat cameraX, cameraY, cameraZ;
GLboolean mouse_is_first = true;
GLdouble lastX = width / 2, lastY = height / 2;
GLfloat Cyaw = -90.0f, Cpitch = 0.0f;
GLfloat pov = 45.0f;
GLfloat cubeX, cubeY, cubeZ;
glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

glm::mat4 modelMatrix()
{
	model = translate(model, glm::vec3(cubeX, cubeY, cubeZ));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	return model;
}
glm::mat4 viewMatrix()
{

	view = lookAt(pos, pos + CF, CU);
	return view;
}
glm::mat4 perMatrix()
{
	GLfloat aspect = GLfloat(width / height);
	GLfloat near = 0.1f;
	GLfloat far = 100.0f;
	projection = glm::perspective(glm::radians(pov*float(sqrt(aspect))), aspect, near, far);
	return projection;
}
void updateUniform()
{
	viewMatrix();
	perMatrix();
}
void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	if (mouse_is_first)
	{
		lastX = xpos;
		lastY = ypos;
		mouse_is_first = false;
	}
	float xoffset = static_cast<float>(xpos - lastX);
	float yoffset = static_cast<float>(lastY - ypos);
	lastX = xpos;
	lastY = ypos;
	Cyaw += xoffset * 0.1f;
	Cpitch += yoffset * 0.1f;
	if (Cpitch > 89.0f)
	{
		Cpitch = 89.0f;
	}
	if (Cpitch < -89.0f)
	{
		Cpitch = -89.0f;
	}
	CF = glm::normalize(glm::vec3(cos(glm::radians(Cyaw)) * cos(glm::radians(Cpitch)),
								  sin(glm::radians(Cpitch)),
								  sin(glm::radians(Cyaw)) * cos(glm::radians(Cpitch))));
	CR = glm::normalize(cross(CF, worldUp));
	CU = cross(CR, CF);
}

void closeCallback(GLFWwindow *window) // 窗口关闭时执行
{
	std::cout << "window close" << std::endl;
}
void reshapeCallback(GLFWwindow *window, int newwidth, int newheight) // 窗口大小改变时执行
{
	width = newwidth;
	height = newheight;
	glViewport(0, 0, width, height);
}
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) // 按键按下时执行，适合快捷键操作
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && action == GLFW_PRESS)
	{
		if (mouseGet)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(window, nullptr);
			mouseGet = false;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			mouse_is_first = true;
			glfwSetCursorPosCallback(window, mouseCallback);
			mouseGet = true;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		keyFirst = true;
	}
}
void keyCallbackLongTime(GLFWwindow *window) // 持续监听键盘，适合连续进行的动作
{
	GLfloat speed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		pos += speed * CF;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		pos -= speed * CF;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		pos -= speed * CR;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		pos += speed * CR;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		pos -= speed * CU;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		pos += speed * CU;
	}
}
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		mouse_is_first = true;
	}
}
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) // 鼠标滚轮滚动时执行
{
	pov -= yoffset;
	if (pov < 1.0f)
	{
		pov = 1.0f;
	}
	if (pov > 70.0f)
	{
		pov = 70.0f;
	}
}
