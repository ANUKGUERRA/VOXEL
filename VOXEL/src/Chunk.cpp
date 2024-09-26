#include "Chunk.h"


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
	for (int z = 0; z < chunkSize; z++) {

		for (int y = 0; y < chunkSize; y++) {

			for (int x = 0; x < chunkSize; x++) {
				blocks[x][y][z].setActive(true);
			}

		}

	}



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

struct VertexHasher {
	std::size_t operator()(const glm::ivec3& v) const {
		return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) ^ std::hash<int>()(v.z);
	}
};


int Chunk::getVertexIndex(const glm::ivec3& vertex) {
	// Check if the vertex already exists
	auto it = vertexMap.find(vertex);
	if (it != vertexMap.end()) {
		return it->second;
	}
	else {
		// Add new vertex and return its index
		int newIndex = vertices.size();
		vertices.push_back(vertex);
		vertexMap[vertex] = newIndex;
		return newIndex;
	}
}

void Chunk::createCube(int i, int j, int k) {
	// Define vertices relative to the cube position
	glm::ivec3 v000(i, j, k);
	glm::ivec3 v001(i, j, k + 1);
	glm::ivec3 v010(i, j + 1, k);
	glm::ivec3 v011(i, j + 1, k + 1);
	glm::ivec3 v100(i + 1, j, k);
	glm::ivec3 v101(i + 1, j, k + 1);
	glm::ivec3 v110(i + 1, j + 1, k);
	glm::ivec3 v111(i + 1, j + 1, k + 1);

	// Create faces with unique vertices
	if (!xPos) {
		indices.push_back(getVertexIndex(v100));
		indices.push_back(getVertexIndex(v101));
		indices.push_back(getVertexIndex(v111));
		indices.push_back(getVertexIndex(v100));
		indices.push_back(getVertexIndex(v111));
		indices.push_back(getVertexIndex(v110));
	}

	if (!xNeg) {
		indices.push_back(getVertexIndex(v000));
		indices.push_back(getVertexIndex(v010));
		indices.push_back(getVertexIndex(v011));
		indices.push_back(getVertexIndex(v000));
		indices.push_back(getVertexIndex(v011));
		indices.push_back(getVertexIndex(v001));
	}

	if (!yPos) {
		indices.push_back(getVertexIndex(v010));
		indices.push_back(getVertexIndex(v110));
		indices.push_back(getVertexIndex(v111));
		indices.push_back(getVertexIndex(v010));
		indices.push_back(getVertexIndex(v111));
		indices.push_back(getVertexIndex(v011));
	}

	if (!yNeg) {
		indices.push_back(getVertexIndex(v000));
		indices.push_back(getVertexIndex(v001));
		indices.push_back(getVertexIndex(v101));
		indices.push_back(getVertexIndex(v000));
		indices.push_back(getVertexIndex(v101));
		indices.push_back(getVertexIndex(v100));
	}

	if (!zPos) {
		indices.push_back(getVertexIndex(v001));
		indices.push_back(getVertexIndex(v011));
		indices.push_back(getVertexIndex(v111));
		indices.push_back(getVertexIndex(v001));
		indices.push_back(getVertexIndex(v111));
		indices.push_back(getVertexIndex(v101));
	}

	if (!zNeg) {
		indices.push_back(getVertexIndex(v000));
		indices.push_back(getVertexIndex(v100));
		indices.push_back(getVertexIndex(v110));
		indices.push_back(getVertexIndex(v000));
		indices.push_back(getVertexIndex(v110));
		indices.push_back(getVertexIndex(v010));
	}

}



void Chunk::init() {
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::ivec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	
	//Position
	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(GLint), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(0);

}

// Render the cube
void Chunk::draw() {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



// Cleanup resources
void Chunk::cleanup() {
	if (EBO) glDeleteBuffers(1, &EBO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}
