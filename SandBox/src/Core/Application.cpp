#include "Application.h"

#define NS_BUILD
#define NS_APP_FRAMEWORK
#define NS_LICENSE



 
Application::Application(){
	initializeGLFW();
	createWindow();
	setupOpenGL();
}

Application::~Application() {
	glfwTerminate();
}

void Application::initializeGLFW()
{
	if (!glfwInit()) {
		throw runtime_error("Failed to initialize GLFW");
	}
}

void Application::createWindow() {
	window = glfwCreateWindow(windowWidth, windowHeight, "Voxel", NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, InputSystem::cursor_position_callback);
	glfwSetMouseButtonCallback(window, InputSystem::mouse_button_callback);
	glfwSetKeyCallback(window, InputSystem::key_callback);

}



void Application::setupOpenGL() {
	if (glewInit() != GLEW_OK) {
		throw runtime_error("Failed to initialize GLEW");
	}


	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glfwSetFramebufferSizeCallback(window, UiSystem::framebuffer_size_callback);
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwSwapInterval(1);
	glClearColor(0.04f, 0.2f, 0.25f, 1.0f);
}


void Application::start() {
	scene = new Scene();
	scene->initialize();
}


void Application::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		calculateDeltaTime();

		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glfwGetCursorPos(window, &mouseX, &mouseY);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		scene->update(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


void Application::calculateDeltaTime() {
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	frameCount++;
	if (currentFrame - lastTime >= 1.0) {

		double fps = double(frameCount) / (currentFrame - lastTime);
		cout << "FPS: " << fps << " | Delta Time: " << deltaTime << " seconds" << endl;

		lastTime = currentFrame;
		frameCount = 0;
	}
}
