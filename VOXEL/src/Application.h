#pragma once

#include "Camera.h"
#include "Shader.h"
#include "Map.h"
#include <GLFW/glfw3.h>

class Application {
private:
    GLFWwindow* window;
    Camera camera;
    
    Map map;
    float deltaTime;
    float lastFrame;

    int widowWidth = 500;
    int windowHeight = 500;

    void initializeGLFW();
    void createWindow();
    void setupOpenGL();

    // Shaders:
    Shader mapShader;


public:
    Application();
    ~Application();
    void mainLoop();
    void processInput();
    static void mouseMoveCallback(GLFWwindow* window, double xposIn, double yposIn);
    void render(glm::vec3 translate, Chunk* chunk);


};
