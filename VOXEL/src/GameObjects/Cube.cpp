#include "Cube.h"

Cube::Cube()
{
    collider = addComponent<ColliderComponent>();
}

const int Cube::cubeIndices[6][6] = {
    // xPos face
    {0, 1, 2, 0, 2, 3},
    // xNeg face           
    {4, 5, 6, 4, 6, 7,},
    // yPos face           
    {8, 9, 10, 8, 10, 11},
    // yNeg face           
    {12, 13, 14, 12, 14, 15},
    // zPos face           
    {16, 17, 18, 16, 18, 19},
    // zNeg face           
    {20, 21, 22, 20, 22, 23}
};
const glm::vec3 Cube::normals[6] = {
	glm::vec3(1.0f, 0.0f, 0.0f),  // Pos X
	glm::vec3(-1.0f, 0.0f, 0.0f), // Neg X

	glm::vec3(0.0f, 1.0f, 0.0f),  // Pos Y
	glm::vec3(0.0f, -1.0f, 0.0f), // Neg Y

	glm::vec3(0.0f, 0.0f, 1.0f),  // Pos Z
	glm::vec3(0.0f, 0.0f, -1.0f)  // Neg Z
};
const glm::vec2 Cube::uv[24] = {
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, // xPos face
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, // xNeg face
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, // yPos face
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, // yNeg face
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, // zPos face
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}  // zNeg face
};
const int Cube::faceVertices[6][4][3] = {
        {{1,0,0}, {1,0,1}, {1,1,1}, {1,1,0}},  // xPos
        {{0,0,0}, {0,1,0}, {0,1,1}, {0,0,1}},  // xNeg
        {{0,1,0}, {1,1,0}, {1,1,1}, {0,1,1}},  // yPos
        {{0,0,0}, {0,0,1}, {1,0,1}, {1,0,0}},  // yNeg
        {{0,0,1}, {0,1,1}, {1,1,1}, {1,0,1}},  // zPos
        {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0}}   // zNeg
};


void Cube::start()
{
    
}
void Cube::update(float deltaTime)
{

}