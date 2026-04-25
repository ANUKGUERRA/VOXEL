#ifndef APPLICATION_H
#define APPLICATION_H

#include "config.h"
#include "../Scene/Systems.h"
#include "../Scene/Scene.h"



class Application {
public:
    Application();
    ~Application();
    void start();
    void mainLoop();
    void calculateDeltaTime();



private:

    Scene* scene;

    void initializeGLFW();
    void createWindow();
    void setupOpenGL();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    double currentFrame;
    int frameCount = 0;
    double lastTime = 0.0;
};

#endif