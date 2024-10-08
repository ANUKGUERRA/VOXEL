#include "Chunk.h"

const glm::vec3 NORMALS[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),  // Pos X
	glm::vec3(-1.0f, 0.0f, 0.0f), // Neg X

	glm::vec3(0.0f, 1.0f, 0.0f),  // Pos Y
	glm::vec3(0.0f, -1.0f, 0.0f), // Neg Y

	glm::vec3(0.0f, 0.0f, 1.0f),  // Pos Z
	glm::vec3(0.0f, 0.0f, -1.0f)  // Neg Z
};


Chunk::Chunk() : VBO(0), VAO(0), EBO(0)
{
	xNeg = false, xPos = false, yNeg = false, yPos = false, zNeg = false, zPos = false;
	blocks = new Cube * *[chunkSize];
	for (int i = 0; i < chunkSize; i++)
	{
		blocks[i] = new Cube * [chunkSize];
		for (int j = 0; j < chunkSize; j++)
		{
			blocks[i][j] = new Cube[chunkSize];
		}
	}
}

Chunk::~Chunk()
{
	for (int i = 0; i < chunkSize; i++)
	{
		for (int j = 0; j < chunkSize; j++)
		{
			delete[] blocks[i][j];
		}
		delete[] blocks[i];
	}
	cleanup();
};

void Chunk::generateChunk()
{
	for (int x = 0; x < chunkSize; x++)
	{
		for (int y = 0; y < chunkSize; y++)
		{
			for (int z = 0; z < chunkSize; z++)
			{
				if (!blocks[x][y][z].isActive()) {
					continue;
				}
				xNeg = false, xPos = false, yNeg = false, yPos = false, zNeg = false, zPos = false;
				if (x > 0) xNeg = blocks[x - 1][y][z].isActive();
				if (x < chunkSize - 1) xPos = blocks[x + 1][y][z].isActive();

				if (y > 0) yNeg = blocks[x][y - 1][z].isActive();
				if (y < chunkSize - 1) yPos = blocks[x][y + 1][z].isActive();

				if (z > 0) zNeg = blocks[x][y][z - 1].isActive();
				if (z < chunkSize - 1) zPos = blocks[x][y][z + 1].isActive();

				createCube(x, y, z);
			}
		}
	}
};


void Chunk::createCube(int x, int y, int z) {
	const bool shouldRenderFace[6] = { !xPos, !xNeg, !yPos, !yNeg, !zPos, !zNeg };

	uint32_t baseIndex = vertices.size();

	for (int face = 0; face < 6; ++face) {
		if (shouldRenderFace[face]) {

			for (int i = 0; i < 4; ++i) {
				glm::vec3 position(
					x + Cube::faceVertices[face][i][0],
					y + Cube::faceVertices[face][i][1],
					z + Cube::faceVertices[face][i][2]
				);
				vertices.push_back({ position, NORMALS[face] });
			}

			indices.push_back(baseIndex);
			indices.push_back(baseIndex + 1);
			indices.push_back(baseIndex + 2);
			indices.push_back(baseIndex);
			indices.push_back(baseIndex + 2);
			indices.push_back(baseIndex + 3);

			baseIndex += 4;
		}
	}
}


void Chunk::init(int chunkX, int chunkZ) {
	generateNoise(chunkX, chunkZ, chunkSize, blocks);
	generateChunk();
	setupBuffers();
}

// Setup OpenGL buffers
void Chunk::setupBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}



// Render the cube
void Chunk::draw() {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



// Cleanup Buffers
void Chunk::cleanup() {
	if (EBO) glDeleteBuffers(1, &EBO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}
