#ifndef APPLICATION_H
#define APPLICATION_H


#include "Shader/Shader.h"
#include "Map/headers/Map.h"
#include "3DModels/ModelLoader.h"
#include <GLFW/glfw3.h>
#include "GameObjects/Player.h"
#include "Components/headers/GameObject.h"
class Application {
private:
    GLFWwindow* window;

    Map map;

    float deltaTime;
    float lastFrame;



    void initializeGLFW();
    void createWindow();
    void setupOpenGL();

    Shader mapShader;
    Shader modelShader;

    /*Model* importedModel;*/


public:
    Application();
    ~Application();
    void mainLoop();
    void processInput();
    static void mouseMoveCallback(GLFWwindow* window, double xposIn, double yposIn);
    glm::vec3 ApplyGravity(glm::vec3 position, glm::vec3 &velocity, float deltaTime, float gravityStrength);

    static Player player;
    static CameraComponent* camera;
    static InputComponent* input;
    static TransformComponent* transform;
    static int widowWidth;
    static int windowHeight;

    //void renderMap(Chunk* chunk);
    //void renderModel();
    //void processCollisions();


};

#endif
