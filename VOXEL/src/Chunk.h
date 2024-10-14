#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "../include/libs.h"
#include "Cube.h"
#include "noise.h"
#include <unordered_map>



class Chunk {

public:
	Chunk();
	~Chunk();
	void generateChunk(int chunkX, int chunkZ);
	void init(int chunkX, int chunkZ);
	void draw();
	void cleanup();
	
	Cube** blocks;
	static const int chunkSize = 64;


private:

	GLuint VBO, VAO, EBO;

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
	};
	
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	static int faceIndices[36];

	bool xNeg, xPos, yNeg, yPos, zNeg, zPos;

	

	
	void setupBuffers();
	void createCube(int i, int j, int k);
};