#pragma once
#include "Collider.h"
class Cube {
public:
    Cube();
    ~Cube();
    bool isActive();
    void setActive(bool active);
    static const int faceVertices[6][4][3];
    static const int cubeIndices[6][6];
    static const glm::vec3 normals[6];
    static const glm::vec2 uv[24];
    Collider collider;
    int height;
private:
    bool active;
    
};