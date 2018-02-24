#define STB_IMAGE_IMPLEMENTATION

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

// Include GLEW, used to detect supported openGL extensions
#include <GL\glew.h>

// Include GLM, math library
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>

//#include "shader.h"
#include "common.h"
#include "camera.h"
#include "game_class.h"
#include "model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void window_focus_callback(GLFWwindow* window, int focused);

Shader ourShader;

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const float SCREEN_RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;
// render time, admin machine 16 sek from pressing debug to show render at 300x300 (for reference)
<<<<<<< HEAD
const unsigned int MAP_WIDTH = 300;
const unsigned int MAP_HEIGHT = 400;
=======
const unsigned int MAP_WIDTH = 120;
const unsigned int MAP_HEIGHT = 80;
>>>>>>> 1d9d83a5e4428b0a569d982194f503e11c64dd58
std::string exe_path;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// camera
Camera camera = Camera();

int main(int argc, char* argv[])
{

	// store path for resources
	exe_path = std::string(argv[0]);
	exe_path = exe_path.substr(0, exe_path.find_last_of("\\/"));
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CityBuildingGame", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);

	camera = Camera(glm::vec3(20.0f, 0.0f, 50.0f), window);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != 0)
	{
		exit(EXIT_FAILURE);
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	// ------------------------------------
	//Shader ourShader("basic_lighting.vert", "basic_lighting.frag"); // you can name your shader files however you like
	Shader ourShader2("vertex_shader.vert", "fragment_shader.frag");
	Shader ourShader("basic_lighting.vert", "basic_lighting.frag");
	Shader shaderTree("vertex_shader.vert", "fragment_shader.frag");
	//unsigned int diffuseMap = Common::loadTexture("Grass.bmp");
	//unsigned int specularMap = Common::loadTexture("container2_specular.png");
	//ourShader.setInt("material.diffuse", 0);
	//ourShader.setInt("material.specular", 1);
	// note: we only call this once since only this shader exists

	// render loop
	// -----------

	GameClass gameClass((float)SCR_WIDTH, (float)SCR_HEIGHT, MAP_WIDTH, MAP_HEIGHT);

	std::string texture_path;
	// nanosuit test model
	//texture_path = exe_path + "\\nanosuit\\nanosuit.obj";
	//std::replace(texture_path.begin(), texture_path.end(), '\\', '/');
	//Model ourModel(texture_path.c_str());
	
	// tree test model
	texture_path = exe_path + "\\tree2_3ds\\Tree2.3ds";
	std::replace(texture_path.begin(), texture_path.end(), '\\', '/');
	Model tree(texture_path.c_str());


	const int TICKS_PER_SECOND = 25;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 5;

	DWORD next_game_tick = GetTickCount();
	int loops;
	float interpolation;
	while (!glfwWindowShouldClose(window))
	{
		loops = 0;
		while (GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) {
			std::thread t(GameClass.UpdateWorld(), &gameClass);
			gameClass.UpdateWorld();

			next_game_tick += SKIP_TICKS;
			loops++;
		}

		ourShader.use();

		// input
		// -----
		processInput(window);

		camera.lock_cursor_to_window();
		camera.mouse_scroll();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// light properties
		glm::vec3 lightColor;
		lightColor.x = 1.0f;//sin(glfwGetTime() * 2.0f);
		lightColor.y = 1.0f;// sin(glfwGetTime() * 0.7f);
		lightColor.z = 1.0f;// sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		ourShader.setVec3("light.ambient", ambientColor);
		ourShader.setVec3("light.diffuse", diffuseColor);
		//ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("light.position", camera.Position);
		ourShader.setVec3("viewPos", camera.Position);

		// material properties
		//ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
		//ourShader.setFloat("material.shininess", 32.0f);
		
		//glm::mat4 projection = glm::ortho(-1.77777f * camera.Zoom, 1.77777f * camera.Zoom, -1 * camera.Zoom, 1 * camera.Zoom, 1.0f, 1000.0f);
		glm::mat4 projection = glm::ortho(-SCREEN_RATIO * camera.Zoom, SCREEN_RATIO * camera.Zoom, -1 * camera.Zoom, 1 * camera.Zoom, 1.0f, 1000.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);



		interpolation = float(GetTickCount() + SKIP_TICKS - next_game_tick)
			/ float(SKIP_TICKS);
		gameClass.Draw(interpolation);



		shaderTree.use();
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(20.0f, 20.0f, 20.0f));
		shaderTree.setMat4("projection", projection);
		shaderTree.setMat4("view", view);
		shaderTree.setMat4("model", model2);

		tree.Draw(shaderTree);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.keyboard_scroll(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.keyboard_scroll(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.keyboard_scroll(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.keyboard_scroll(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.mouse_zoom((float)yoffset);
}

// glfw: whenever the window receives focus, camera gets locked
void window_focus_callback(GLFWwindow *window, int focused)
{
	if (focused) {
		camera.lock_cursor_to_window();
	}
}