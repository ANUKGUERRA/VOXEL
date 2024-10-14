#pragma once
#include <string>
#include <vec3.hpp>
#include "Shader.h"

class Colider
{
public:
	Colider(){}
	Colider(glm::vec3 min, glm::vec3 max) : min(min), max(max)
	{
		//coliderDrawSetUp();
	}

	
	void coliderDraw();
	void setColiderPosition(glm::vec3 min, glm::vec3 max);

	bool intersects(const Colider& other)const;

	std::string Tag;
	glm::vec3 min;
	glm::vec3 max;


private:
	unsigned int VAO, VBO, EBO;
	void coliderDrawSetUp();

};