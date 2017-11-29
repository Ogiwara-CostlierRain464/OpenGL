#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <common/shader.hpp>

using namespace glm;

GLFWwindow* window;
GLuint vbo1;

bool init() {
	if (!glfwInit()) {
		return false;
	}

	window = glfwCreateWindow(1024, 768, "OpenGL", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	printf("初期化完了");

	return true;
}

void vertex() {
	glGenVertexArrays(1, &vbo1);
	glBindVertexArray(vbo1);
}

void mainLoop() {
	do {

		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	printf("メインループ終了");
}

int main() {

	if (!init())
		exit(0);

	vertex();

	mainLoop();

	glfwTerminate();

	return 0;
}
