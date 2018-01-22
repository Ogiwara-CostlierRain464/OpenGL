#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.cpp>
#include <common/objloader.cpp>
#include <common/vboindexer.cpp>

#define VS "VS.shader"
#define FS "FS.shader"
#define DDS "uvmap.DDS"
#define OBJ "suzanne.obj"
using namespace std;

GLFWwindow* window;

GLuint programId;

GLuint matrixId;
GLuint viewMatrixId;
GLuint modelMatrixId;

GLuint texture;
GLuint textureId;

vector<vec3> vertices;
vector<vec2> uvs;
vector<vec3> normals; //法線

GLuint vertexArraayId;

GLuint vertexBuffer;
GLuint uvBuffer;
GLuint normalBuffer;
GLuint elementBuffer;

GLuint lightId;

vector<unsigned short> indices;
vector<vec3> indexedVertices;
vector<vec2> indexedUvs;
vector<vec3> indexedNormals;


glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void computeMatricesFromInputs();

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	glClearColor(256.0f, 256.0f, 256.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	printf("初期化完了");

	return true;
}

void vertex() {
	glGenVertexArrays(1, &vertexArraayId);
	glBindVertexArray(vertexArraayId);

	programId = LoadShaders(VS,FS);
	matrixId = glGetUniformLocation(programId,"mvp");
	viewMatrixId = glGetUniformLocation(programId, "v");
	modelMatrixId = glGetUniformLocation(programId, "m");

	texture = loadDDS(DDS);
	textureId = glGetUniformLocation(programId, "textureSampler");

	loadOBJ(OBJ, vertices, uvs , normals);

	indexVBO(vertices, uvs, normals, indices, indexedVertices, indexedUvs, indexedNormals);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedVertices.size() * sizeof(vec3), &indexedVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedUvs.size() * sizeof(vec2), &indexedUvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexedNormals.size() * sizeof(vec3), &indexedNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glUseProgram(programId);
	lightId = glGetUniformLocation(programId, "lightPositionWorldSpace");
}

void mainLoop() {
	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programId);

		computeMatricesFromInputs();
		mat4 projectMat = getProjectionMatrix();
		mat4 viewMat = getViewMatrix();
		mat4 modelMat = mat4(1.0); //E
		mat4 mvp = projectMat * viewMat * modelMat;

		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &modelMat[0][0]);
		glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, &viewMat[0][0]);

		vec3 lightPos = vec3(4, 4, 4);
		glUniform3f(lightId, lightPos.x, lightPos.y, lightPos.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(textureId, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  
			3,GL_FLOAT,GL_FALSE,0,(void*)0);


		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(
			1,
			2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(
			2,
			3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

		glDrawElements(
			GL_TRIANGLES,
			indices.size(),
			GL_UNSIGNED_SHORT,
			(void*)0
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (
		glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &elementBuffer);

	glDeleteProgram(programId);

	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &vertexArraayId);

	printf("メインループ終了");
}

int main() {

	if (!init()) {
		fprintf(stderr, "INIT FAILED!");
		exit(0);
	}

	vertex();

	mainLoop();

	glfwTerminate();

	return 0;
}
