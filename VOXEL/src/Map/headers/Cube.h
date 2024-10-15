#pragma once
#include "../../Collider.h"
class Cube {
public:
    Cube();
    ~Cube();
    bool isActive();
    void setActive(bool active);
    static const int faceVertices[6][4][3];
    static const int cubeIndices[6][6];
    Collider collider;
    int height;
private:
    bool active;
    
};