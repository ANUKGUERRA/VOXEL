#pragma once

#include "Camera.h"
#include "Shader.h"
#include "Map.h"
#include "ModelLoader.h"
#include <GLFW/glfw3.h>

class Application {
private:
    GLFWwindow* window;
    Camera camera;
    
    Map map;
    float deltaTime;
    float lastFrame;

    int widowWidth = 900;
    int windowHeight = 800;

    void initializeGLFW();
    void createWindow();
    void setupOpenGL();

    Shader mapShader;
    Shader modelShader;

    Model* importedModel;


public:
    Application();
    ~Application();
    void mainLoop();
    void processInput();
    static void mouseMoveCallback(GLFWwindow* window, double xposIn, double yposIn);
    void renderMap(Chunk* chunk);
    void renderModel();


};
