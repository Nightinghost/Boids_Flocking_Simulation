

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Other includes

#include "Shader.h"
#include <vector>
#include <time.h>
#include "Boid.h"
#include "Flock.h"

using namespace std;


GLint WIDTH = 800, HEIGHT = 600;

Flock f(100);

std::vector<float> position_2d;
GLuint BOID, VAO;

// Flocking mode
Boid::Flag mode = Boid::NOCONTROL;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main() {	

	position_2d = f.get_position_2d_normalized();
	float points_num = position_2d.size() / 2;

	//// 初始并配置glfw库
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	
	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Flocking Control Simulate", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	// 初始化glew库
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// 着色器
	Shader boid_shader("./boid.vert", "./boid.frag");

	// 绑定数据
	glGenBuffers(1, &BOID);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, BOID);
	glBufferData(GL_ARRAY_BUFFER, position_2d.size() * sizeof(float), &position_2d.front(), GL_DYNAMIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLfloat*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// -------------------------------------
	// 主循环
	while (!glfwWindowShouldClose(window)) {
		// 检查事件
		glfwPollEvents();

		//-----------------------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPointSize(5.0f);
		boid_shader.Use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, points_num);
		glBindVertexArray(0);

	
		// 交换缓冲
		glfwSwapBuffers(window);

		// 运行一次模拟
		position_2d = f.update(mode);
		glBindBuffer(GL_ARRAY_BUFFER, BOID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, position_2d.size() * sizeof(float), &position_2d.front());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_A:
		mode = Boid::ALIGNMENT;
		break;
	case GLFW_KEY_C:
		mode = Boid::COHESION;
		break;
	case GLFW_KEY_S:
		mode = Boid::SEPARATION;
		break;
	case GLFW_KEY_F:
		mode = Boid::FLOCKING;
		break;
	default:
		mode = Boid::NOCONTROL;
		break;
	}
}