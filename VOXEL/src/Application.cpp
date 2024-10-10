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

	importedModel = new Model("res/3DModels/Livinglegend.fbx");

	mapShader.load("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
	modelShader.load("res/shaders/modelVertex.glsl", "res/shaders/modelFragment.glsl");
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
	window = glfwCreateWindow(widowWidth, windowHeight, "Voxel", NULL, NULL);
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


	glfwGetFramebufferSize(window, &widowWidth, &windowHeight);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, widowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwSwapInterval(1);

	
}

double lastTime = glfwGetTime();
double lastFrame = glfwGetTime();
double currentFrame = glfwGetTime();
double deltaTime = 0.0f;
int frameCount = 0;

void Application::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwGetFramebufferSize(window, &widowWidth, &windowHeight);
		glViewport(0, 0, widowWidth, windowHeight);
		//Sky color Cambiar per SkyBox
		glClearColor(0.04f, 0.2f, 0.25f, 1.0f);
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

		renderModel();


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

void Application::renderMap(Chunk* chunk)
{
	glUseProgram(mapShader.program);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)widowWidth / (float)windowHeight, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);


	mapShader.setMat4("projection", projection);
	mapShader.setMat4("view", view);
	mapShader.setMat4("model", model);

	mapShader.setVec3("lightDir", glm::vec3(0.7f, -0.4f, 0.6f));
	mapShader.setVec3("viewPos", camera.Position);
	mapShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f)); 
	mapShader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));


	chunk->draw();

	glUseProgram(0);
}

void Application::renderModel() 
{

	glUseProgram(modelShader.program);

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)widowWidth / (float)windowHeight, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 35.0f, 0.0f)); 
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f, 0.02f, 0.02f)); 

	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);
	modelShader.setMat4("model", modelMatrix);
	modelShader.setVec3("viewPos", camera.Position);


	modelShader.setVec3("light.position", glm::vec3(0.7f, 0.2f, 2.0f));
	modelShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	modelShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	modelShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	importedModel->draw(modelShader);

	glUseProgram(0);
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
