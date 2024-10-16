#pragma once

#include "Shader/Shader.h"
#include "Map/headers/Map.h"
#include "3DModels/ModelLoader.h"
#include <GLFW/glfw3.h>
#include "GameObjects/Player.h"

class Application {
private:
    GLFWwindow* window;

    Player player;
    CameraComponent *camera = player.getComponent<CameraComponent>();
    InputComponent* input = player.getComponent<InputComponent>();
    TransformComponent *transform = player.getComponent<TransformComponent>();

    
    
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
