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
    Collider playerCollider;
    Collider playerGroundCollider;
    
    Map map;
    float deltaTime;
    float lastFrame;

    static int widowWidth;
    static int windowHeight;

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
    glm::vec3 ApplyGravity(glm::vec3 position, glm::vec3 &velocity, float deltaTime, float gravityStrength);
    void renderMap(Chunk* chunk);
    void renderModel();
    void processCollisions();


};
