// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "ground.h"
#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768


#include "specularlight.h"




Texture* m_pTexture;

GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
int verticenumber;


static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

GLuint m_VBO;

void CreateVertexBuffer()
{
	const glm::vec3 Normal = glm::vec3(0.0, 1.0f, 0.0f);

	Vertex Vertices[6] = {
		Vertex(glm::vec3(0.0f, 0.0f, 0.0f),             glm::vec2(0.0f, 0.0f), Normal),
		Vertex(glm::vec3(0.0f, 0.0f, FieldDepth),       glm::vec2(0.0f, 1.0f), Normal),
		Vertex(glm::vec3(FieldWidth, 0.0f, 0.0f),       glm::vec2(1.0f, 0.0f), Normal),

		Vertex(glm::vec3(FieldWidth, 0.0f, 0.0f),       glm::vec2(1.0f, 0.0f), Normal),
		Vertex(glm::vec3(0.0f, 0.0f, FieldDepth),       glm::vec2(0.0f, 1.0f), Normal),
		Vertex(glm::vec3(FieldWidth, 0.0f, FieldDepth), glm::vec2(1.0f, 1.0f), Normal)
	};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void init()
{
	LightInit();
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	CreateVertexBuffer();


	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	



	SpecularShader.shader = LoadShaders("shader/specular_vert.glsl", "shader/specular_fragment.glsl");
	if (!SpecularShader.Init())
	{
		std::cout << "SpecularShader ERROR!" << std::endl;
		exit(-1);
	}

	SpecularShader.Enable();

	SpecularShader.SetTextureUnit(0);



	m_pTexture = new Texture(GL_TEXTURE_2D, "test.png");

	if (!m_pTexture->Load()) {
		std::cout << "Texture ERROR!" << std::endl;
		exit(-1);
	}

}


void render()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glm::vec3 camerapos = getCameraPosition();

	SpecularShader.Enable();
	SpecularShader.SetWVP(MVP);
	SpecularShader.SetWorldMatrix(ModelMatrix);
	SpecularShader.SetDirectionalLight(m_directionalLight);
	SpecularShader.SetEyeWorldPos(camerapos);
	SpecularShader.SetMatSpecularIntensity(1.0f);
	SpecularShader.SetMatSpecularPower(32);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	m_pTexture->Bind(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_A:
		m_directionalLight.AmbientIntensity += 0.05f;
		break;
	case GLFW_KEY_S:
		m_directionalLight.AmbientIntensity -= 0.05f;
		break;
	case GLFW_KEY_Z:
		m_directionalLight.DiffuseIntensity += 0.05f;
		break;
	case GLFW_KEY_X:
		m_directionalLight.DiffuseIntensity -= 0.05f;
		break;
	}
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tutorial 01", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
							 // Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyCallback);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	init();

	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw nothing, see you in tutorial 2 !

		render();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	delete m_pTexture;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

