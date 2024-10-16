#pragma once
#include "../Components/headers/Component.h"
#include "../Components/headers/ColliderComponent.h"
#include <vec2.hpp>
class Cube : public GameObject
{
public:
	Cube();
	ColliderComponent *collider;

	static const int faceVertices[6][4][3];
	static const glm::vec3 normals[6];
	static const glm::vec2 uv[24];
	static const int cubeIndices[6][6];
	int height;

	void start() override;
	void update(float deltaTime) override;
};

