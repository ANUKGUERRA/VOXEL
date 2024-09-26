#include "Application.h"
#include "../include/libs.h"
#include "noise.h"

Application::Application()
	: window(nullptr),
	camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f), deltaTime(0.0f), lastFrame(0.0f) {
	initializeGLFW();
	createWindow();
	setupOpenGL();
	
	map.loadMap();
}

Application::~Application() {
	glfwTerminate();
}

void Application::initializeGLFW()
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
}

void Application::createWindow() {
	window = glfwCreateWindow(1920 / 2, 1080 / 2, "Voxel", NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
}

void Application::setupOpenGL() {
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW");
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSwapInterval(1);

	shader.load("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
}

double lastTime = glfwGetTime();
double lastFrame = glfwGetTime();
double currentFrame = glfwGetTime();
double deltaTime = 0.0f;
int frameCount = 0;

void Application::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		frameCount++;
		if (currentFrame - lastTime >= 1.0) {
			double fps = double(frameCount) / (currentFrame - lastTime);
			std::cout << "FPS: " << fps << " | Delta Time: " << deltaTime << " seconds" << std::endl;

			lastTime = currentFrame;
			frameCount = 0;
		}

		processInput();
		
		map.updateMap(camera.Position.x, camera.Position.z, *this);


		glfwSwapBuffers(window);
		glfwPollEvents();		
	}
}

void Application::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void Application::render(glm::vec3 translate, Chunk* chunk)
{
	glUseProgram(shader.program);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)1920 / (float)1080, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, translate);

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);

	chunk->draw();
}

void Application::mouseMoveCallback(GLFWwindow* window, double xposIn, double yposIn) {
	static bool firstMouse = true;
	static float lastX = 1920.0f / 2.0;
	static float lastY = 1080.0 / 2.0;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	app->camera.ProcessMouseMovement(xoffset, yoffset);
}
