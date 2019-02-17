/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

#include <sstream>

#include "Utils.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "Camera.h"
#include "Geometry.h"
#include "LightManager.h"
#include "LambertMaterial.h"
#include "PBRMaterial.h"
#include "Texture.h"
#include "TextureMaterial.h"



/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
static void mouseKeyCallback(GLFWwindow* window, int button, int action, int mods);
static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
static void perFrameUniforms(std::vector<std::shared_ptr<Shader>>& shaders, Camera& camera);

/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */
float _zoom = 8;
bool _dragging = false;
bool _strafing = false;
bool _wireframe = false;
bool _backFaceCulling = true;

/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */

	INIReader reader("assets/settings.ini");

	int window_width = reader.GetInteger("window", "width", 800);
	int window_height = reader.GetInteger("window", "height", 800);
	float fov = float(reader.GetReal("camera", "fov", 60.0f));
	float nearZ = float(reader.GetReal("camera", "near", 0.1f));
	float farZ = float(reader.GetReal("camera", "far", 100.0f));
	int refreshRate = reader.GetInteger("window", "refresh_rate", 120);
	std::string windowTitle = reader.Get("window", "title", "ECG");


	/* --------------------------------------------- */
	// Create Window Context
	/* --------------------------------------------- */

	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init glfw");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Request OpenGL version 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Request core profile													  
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Prevent window resizing because viewport would have to resize as well (-> not needed in this course)
	glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);

	//Debug Context

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, windowTitle.c_str(), nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to create window")
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	GLenum error = glewInit();

	if (error != GLEW_OK) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to init glew");
	}

#if _DEBUG
	//Register Callback function
	glDebugMessageCallback(DebugCallbackDefault, NULL);
	//Synchronuous Callback - imidiatly after error
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */

	if (!initFramework()) {
		EXIT_WITH_ERROR("Failed to init framework")
	}

	//Set input callbacks
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseKeyCallback);

	//Set GL defaults (color etc.)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Depth Test
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	{
		//Shaders
		std::vector<std::shared_ptr<Shader>> shaders;

		std::shared_ptr<Shader> simpleTexture = std::make_shared<Shader>("diffuseTexture.vert", "diffuseTexture.frag");
		shaders.emplace_back(simpleTexture);
		std::shared_ptr<Shader> phongPBR = std::make_shared<Shader>("PBR_shader_phong.vert", "PBR_shader_phong.frag");
		shaders.emplace_back(phongPBR);
		//Textures
		std::shared_ptr<Texture> brickTexture = std::make_shared<Texture>("./assets/textures/bricks_diffuse.dds");
		std::shared_ptr<Texture> woodTexture = std::make_shared<Texture>("./assets/textures/wood_texture.dds");

		std::shared_ptr<Material> difTexCube = std::make_shared<TextureMaterial>(simpleTexture,0.1f,0.7f,0.1f,2.0f,woodTexture);
		std::shared_ptr<Material> difTexBricks = std::make_shared<TextureMaterial>(simpleTexture,0.1f,0.7f,0.3f,8.0f,brickTexture);

		

		//Lights
		LightManager lightManager;
		lightManager.createPointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.4f, 1.0f));
		lightManager.createDirectionalLight(glm::vec3(0.8f), glm::vec3(0.0f, -1.0f, -1.0f));

		//Geometry task 5
		glm::mat4 texturedCubeMM = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
		Geometry texturedCube(texturedCubeMM, Geometry::createCubeGeometry(1.5f, 1.5f, 1.5), difTexCube);

		glm::mat4 texturedCylinderMM = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.0f, 0.0f));
		Geometry texturedCylinder(texturedCylinderMM, Geometry::createCylinderGeometry(1.0f, 1.3f, 32), difTexBricks);

		glm::mat4 texturedSphereMM = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -1.0f, 0.0f));
		Geometry texturedSphere(texturedSphereMM, Geometry::createSphereGeometry(1.0f, 64, 32), difTexBricks);


		//Camera
		Camera camera(fov, float(window_width) / float(window_height), nearZ, farZ);
		double mouseX, mouseY;
		double thisFrameTime = 0, oldFrameTime = 0, deltaT = 0;
		double startTime = glfwGetTime();
		unsigned long framecounter = 0;
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//timings
			++framecounter;
			thisFrameTime = glfwGetTime();
			deltaT = thisFrameTime - oldFrameTime;
			oldFrameTime = thisFrameTime;

			//Poll Input Events
			glfwPollEvents();
			glfwGetCursorPos(window, &mouseX, &mouseY);
			//update camera
			camera.update(int(mouseX), int(mouseY), _zoom, _dragging, _strafing);

			//Update Lights
			lightManager.setUniforms(shaders);

			//Update Frame uniforms
			perFrameUniforms(shaders, camera);

			//draw Geometries
			texturedCube.draw();
			texturedCylinder.draw();
			texturedSphere.draw();


			//Swap Buffers
			glfwSwapBuffers(window);
		}
		/* --------------------------------------------- */
		// Statistics
		/* --------------------------------------------- */
		double endTime = glfwGetTime();
		std::cout << "Program ran for " << endTime - startTime << "seconds." << std::endl;
		std::cout << "Average fps: " << framecounter / (endTime - startTime) << "." << std::endl;
	}




	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */

	destroyFramework();


	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */
	
	glfwTerminate();

	return EXIT_SUCCESS;
}


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action != GLFW_RELEASE) return;

	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_F1)
	{
		_wireframe = !_wireframe;
		if (_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	if (key == GLFW_KEY_F2)
	{
		_backFaceCulling = !_backFaceCulling;
		if (_backFaceCulling)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	_zoom -= 0.4f*float(yoffset);
}

static void mouseKeyCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		_dragging = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		_dragging = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		_strafing = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		_strafing = false;
	}
}

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	switch (source) {
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}

void perFrameUniforms(std::vector<std::shared_ptr<Shader>>& shaders, Camera & camera)
{
	for (std::shared_ptr<Shader> shader : shaders) {
		shader->use();
		shader->setUniform("viewProjectionMatrix", camera.getViewProjectionMatrix());
		shader->setUniform("cameraPosition", camera.getPosition());
	}
}
