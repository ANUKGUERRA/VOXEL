#pragma once
#include <string>
#include <vec3.hpp>
#include "Shader/Shader.h"

class Collider
{
public:
	Collider() {}
	Collider(glm::vec3 min, glm::vec3 max) : min(min), max(max)
	{
		//colliderDrawSetUp();
	}


	void colliderDraw();
	void setColliderPosition(glm::vec3 min, glm::vec3 max);

	bool intersects(const Collider& other)const;

	std::string Tag;
	glm::vec3 min;
	glm::vec3 max;


private:
	unsigned int VAO, VBO, EBO;
	void colliderDrawSetUp();
};