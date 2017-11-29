// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW(OpenGL Extension Wrangler Library OpenGL拡張をロードしてくれるライブラリ)
#include <GL/glew.h>

// Include GLFW(Window簡単に作れる奴)
#include <glfw3.h>
GLFWwindow* window;

// Include GLM(Open GL Math)
#include <glm/glm.hpp>
using namespace glm;

int main( void )
{
	//GLFWの初期化
	if( !glfwInit() )
		return -1;
	

	glfwWindowHint(GLFW_SAMPLES, 4); //アンチエイリアス4x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL3.3を使う
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 古いOpenGLは使いません。

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Window Title", NULL, NULL);
	if( window == NULL ){
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	do{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear( GL_COLOR_BUFFER_BIT );

		// Draw nothing, see you in tutorial 2 !

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	getchar();

	return 0;
}

