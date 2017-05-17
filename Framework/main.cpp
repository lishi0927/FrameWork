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
#include "terrain.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 720


#include "specularlight.h"
#include "ImageCompare.h"


Terrain m_terrain;
Camera camera;
bool keys[1024];
int AOflag = 0;
int Textureflag = 1;
int Shadowflag = 1;

void init()
{
	// Enable depth test
//	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
//	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
//	glEnable(GL_CULL_FACE);


	camera.setGrid(GRID);

	camera.setPos(20, 300, 138);
	camera.rotate(-120, 0);
	m_terrain.Init();
}


void render()
{
	glClearDepth(1.0f);
	glClearColor(0.46f, 0.46f, 1.0f, 1.0f);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT,
		znear, zfar);

	glm::mat4 testprojection = glm::mat4(1.0 / tan(glm::radians(fov / 2)) / ((float)WIDTH / (float)HEIGHT), 0.0, 0.0, 0.0,
		0.0, 1.0 / tan(glm::radians(fov / 2)), 0.0, 0.0,
		0.0, 0.0,(znear + zfar) / (zfar - znear), - 2.0* znear * zfar / (zfar - znear),
		0.0, 0.0, 1.0, 0.0);

	testprojection = glm::transpose(testprojection);


	glm::vec3 dir = camera.getDir();
	glm::vec3 up = camera.getUp();
	glm::vec3 pos = camera.getPos();
	pos.x = 0;
	pos.z = 0;

	glm::mat4 view = glm::lookAt(pos, pos + dir, up);
	
	glm::mat4 testview = ViewProjection(pos, pos + dir, up);

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0, -pos.y, 0));

	m_terrain.m_WVP = projection * view * model;

	m_terrain.m_View = view;
	m_terrain.m_World = model;
	m_terrain.currentPos = camera.getPos();
	m_terrain.m_camerapos = glm::vec3(pos.x, pos.y,pos.z);
	m_terrain.m_ar = 1.0 * HEIGHT / WIDTH;
	m_terrain.m_fov = fov;

	m_terrain.render();
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		keys[key] = true;
	else
		if (action == GLFW_RELEASE)
			keys[key] = false;


	if (keys[GLFW_KEY_P])
		printf("Current Positon: %d %d %d\n", int(camera.getPos().x),
			int(camera.getPos().z), int(camera.getPos().y));

	if (keys[GLFW_KEY_O])
		camera.setPos(585, 100, 441);
	//camera.setPos(468, 62, 51);

	if (keys[GLFW_KEY_I])
	{
		AOflag = 1 - AOflag;
		m_terrain.AOflag = AOflag;
	}

	if (keys[GLFW_KEY_U])
	{
		Shadowflag = 1 - Shadowflag;
		m_terrain.m_Shadowflag = Shadowflag;
	}

	if (keys[GLFW_KEY_T])
	{
		Textureflag = 1 - Textureflag;
		m_terrain.m_Textureflag = Textureflag;
	}

	if (keys[GLFW_KEY_ESCAPE])
		exit(0);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static double lastY = 0;
	camera.rotate(xpos, ypos - lastY);
	lastY = ypos;
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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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
//	glfwSetCursorPosCallback(window, mouse_callback);

	// Set the mouse at the center of the screen
	//glfwPollEvents();
	//glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	
	memset(keys, 0, sizeof(keys));

	glViewport(0, 0, WIDTH, HEIGHT);

	init();


	GLfloat lastTime = glfwGetTime();
	GLfloat currentTime;
	

	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		currentTime = glfwGetTime();
		camera.revise();
		camera.move(keys, currentTime - lastTime, CHUNKNUMBER * CHUNKSIZE,
			CHUNKNUMBER * CHUNKSIZE);
		lastTime = currentTime;

		// Draw nothing, see you in tutorial 2 !

		render();

		// Swap buffers
		glfwSwapBuffers(window);

	} // Check if the ESC key was pressed or the window was closed
	while (glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

