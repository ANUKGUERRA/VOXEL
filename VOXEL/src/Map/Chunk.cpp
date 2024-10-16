//#include "headers/Chunk.h"
//#include <iostream>
//
//const glm::vec3 NORMALS[] = {
//	glm::vec3(1.0f, 0.0f, 0.0f),  // Pos X
//	glm::vec3(-1.0f, 0.0f, 0.0f), // Neg X
//
//	glm::vec3(0.0f, 1.0f, 0.0f),  // Pos Y
//	glm::vec3(0.0f, -1.0f, 0.0f), // Neg Y
//
//	glm::vec3(0.0f, 0.0f, 1.0f),  // Pos Z
//	glm::vec3(0.0f, 0.0f, -1.0f)  // Neg Z
//};
//
//
//Chunk::Chunk() : VBO(0), VAO(0), EBO(0)
//{
//	xNeg = false, xPos = false, yNeg = false, yPos = false, zNeg = false, zPos = false;
//	blocks = new Cube *[chunkSize];
//	for (int i = 0; i < chunkSize; i++)
//	{
//		blocks[i] = new Cube [chunkSize];
//	}
//}
//
//Chunk::~Chunk()
//{
//	for (int i = 0; i < chunkSize; i++)
//	{
//		delete[] blocks[i];
//	}
//	delete[] blocks;
//	cleanup();
//};
//
//void Chunk::generateChunk(int chunkX, int chunkZ)
//{
//	for (int x = 0; x < chunkSize; x++)
//	{
//		for (int z = 0; z < chunkSize; z++)
//		{
//			int height = blocks[x][z].height;
//
//			xNeg = false, xPos = false, yNeg = false, yPos = false, zNeg = false, zPos = false;
//
//			if (x > 0)
//				xNeg = (height == blocks[x - 1][z].height);
//			if (x < chunkSize - 1)
//				xPos = (height == blocks[x + 1][z].height);
//
//			if (z > 0)
//				zNeg = (height == blocks[x][z - 1].height);
//			if (z < chunkSize - 1)
//				zPos = (height == blocks[x][z + 1].height);
//
//			createCube(x + chunkX, height, z + chunkZ);
//			blocks[x][z].collider = Collider(glm::vec3(x + chunkX, height, z + chunkZ),glm::vec3(x + chunkX + 1, height + 1, z + chunkZ + 1));
//		}
//	}
//}
//
//
//
//void Chunk::createCube(int x, int y, int z) {
//	const bool shouldRenderFace[6] = { !xPos, !xNeg, !yPos, yNeg, !zPos, !zNeg };
//
//	uint32_t baseIndex = vertices.size();
//
//	for (int face = 0; face < 6; ++face) {
//		if (shouldRenderFace[face]) {
//
//			for (int i = 0; i < 4; ++i) {
//				glm::vec3 position(
//					x + Cube::faceVertices[face][i][0],
//					y + Cube::faceVertices[face][i][1],
//					z + Cube::faceVertices[face][i][2]
//				);
//				vertices.push_back({ position, NORMALS[face] });
//			}
//
//			indices.push_back(baseIndex);
//			indices.push_back(baseIndex + 1);
//			indices.push_back(baseIndex + 2);
//			indices.push_back(baseIndex);
//			indices.push_back(baseIndex + 2);
//			indices.push_back(baseIndex + 3);
//
//			baseIndex += 4;
//		}
//	}
//}
//
//
//void Chunk::init(int chunkX, int chunkZ) {
//	generateNoise(chunkX, chunkZ, chunkSize, blocks);
//	generateChunk(chunkX * chunkSize, chunkZ * chunkSize);
//	setupBuffers();
//}
//
//
//void Chunk::setupBuffers() {
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
//	glEnableVertexAttribArray(1);
//
//	glBindVertexArray(0);
//}
//
//
//void Chunk::draw() 
//{
//    /*for (int x = 0; x < chunkSize; x++)
//    {
//		for (int z = 0; z < chunkSize; z++)
//		{
//			if (blocks[x][z].height == 0)
//				continue;
//            blocks[x][z].collider.colliderDraw();
//        }
//    }*/
//
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}
//
//
//
//
//// Cleanup Buffers
//void Chunk::cleanup() {
//	if (EBO) glDeleteBuffers(1, &EBO);
//	if (VBO) glDeleteBuffers(1, &VBO);
//	if (VAO) glDeleteVertexArrays(1, &VAO);
//}
