#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "../include/libs.h"
#include "Block.h"
#include "noise.h"
#include <unordered_map>



class Chunk {

public:
	Chunk();
	~Chunk();
	void generateChunk();
	void init(int chunkX, int chunkZ);
	void draw();
	void cleanup();
	

	static const int chunkSize = 64;


private:

	GLuint VBO, VAO, EBO;

	struct Vertex {
		glm::ivec3 position;
		glm::vec3 normal;
	};
	
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	static int faceIndices[36];

	bool xNeg, xPos, yNeg, yPos, zNeg, zPos;

	Block*** blocks;

	
	void setupBuffers();
	void createCube(int i, int j, int k);

	void addVertex(const glm::ivec3& position, const glm::vec3& normal);

	

};