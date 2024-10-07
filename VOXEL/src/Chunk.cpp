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
	blocks = new Block * *[chunkSize];
	for (int i = 0; i < chunkSize; i++)
	{
		blocks[i] = new Block * [chunkSize];
		for (int j = 0; j < chunkSize; j++)
		{
			blocks[i][j] = new Block[chunkSize];
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


void Chunk::addVertex(const glm::ivec3& position, const glm::vec3& normal) 
{
	indices.push_back(vertices.size());
	vertices.push_back({ position, normal });
}

void Chunk::createCube(int x, int y, int z) {
	glm::ivec3 v000(x, y, z);
	glm::ivec3 v001(x, y, z + 1);
	glm::ivec3 v010(x, y + 1, z);
	glm::ivec3 v011(x, y + 1, z + 1);
	glm::ivec3 v100(x + 1, y, z);
	glm::ivec3 v101(x + 1, y, z + 1);
	glm::ivec3 v110(x + 1, y + 1, z);
	glm::ivec3 v111(x + 1, y + 1, z + 1);

	// Create faces with unique vertices
	if (!xPos) {
		glm::vec3 normal = NORMALS[0];
		addVertex(v100, normal);
		addVertex(v101, normal);
		addVertex(v111, normal);
		addVertex(v100, normal);
		addVertex(v111, normal);
		addVertex(v110, normal);
	}

	if (!xNeg) {
		glm::vec3 normal = NORMALS[1];
		addVertex(v000, normal);
		addVertex(v010, normal);
		addVertex(v011, normal);
		addVertex(v000, normal);
		addVertex(v011, normal);
		addVertex(v001, normal);
	}

	if (!yPos) {
		glm::vec3 normal = NORMALS[2];
		addVertex(v010, normal);
		addVertex(v110, normal);
		addVertex(v111, normal);
		addVertex(v010, normal);
		addVertex(v111, normal);
		addVertex(v011, normal);
	}

	if (!yNeg) {
		glm::vec3 normal = NORMALS[3];
		addVertex(v000, normal);
		addVertex(v001, normal);
		addVertex(v101, normal);
		addVertex(v000, normal);
		addVertex(v101, normal);
		addVertex(v100, normal);
	}

	if (!zPos) {
		glm::vec3 normal = NORMALS[4];
		addVertex(v001, normal);
		addVertex(v011, normal);
		addVertex(v111, normal);
		addVertex(v001, normal);
		addVertex(v111, normal);
		addVertex(v101, normal);
	}

	if (!zNeg) {
		glm::vec3 normal = NORMALS[5];
		addVertex(v000, normal);
		addVertex(v100, normal);
		addVertex(v110, normal);
		addVertex(v000, normal);
		addVertex(v110, normal);
		addVertex(v010, normal);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	

	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_INT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,normal));
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
