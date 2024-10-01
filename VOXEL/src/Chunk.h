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
	

	static const int chunkSize = 8;


private:
	struct VertexHasher {
		std::size_t operator()(const glm::ivec3& v) const {
			return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) ^ std::hash<int>()(v.z);
		}
	};

	GLuint VBO, VAO, EBO;
	
	std::vector<glm::ivec3> vertices;
	std::vector<int> indices;
	std::unordered_map<glm::ivec3, int, VertexHasher> vertexMap;
	static int faceIndices[36];

	bool xNeg, xPos, yNeg, yPos, zNeg, zPos;

	Block*** blocks;

	
	void setupBuffers();
	void createCube(int i, int j, int k);

	int getVertexIndex(const glm::ivec3& vertex);

	

};