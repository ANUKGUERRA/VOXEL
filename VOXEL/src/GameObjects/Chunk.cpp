#include "Chunk.h"

Chunk::Chunk(int chunkX, int chunkY) : chunkX(chunkX), chunkY(chunkY)
{
	cubes = new Cube *[chunkSize];
	for (int i = 0; i < chunkSize; i++)
		cubes[i] = new Cube[chunkSize];
}
Chunk::~Chunk()
{
	for (int i = 0; i < chunkSize; i++)
		delete[] cubes[i];
	delete[] cubes;
}

void Chunk::generateChunk(int chunkX, int chunkZ)
{
	for (int x = 0; x < chunkSize; x++) 
	{
		for (int z = 0; z < chunkSize; z++) {
			int height = cubes[x][z].height;
			xNeg = false, xPos = false, yNeg = false, yPos = false, zNeg = false, zPos = false;

			if (x > 0)
				xNeg = (height == cubes[x - 1][z].height);
			if (x < chunkSize - 1)
				xPos = (height == cubes[x + 1][z].height);
				
			if (z > 0)
				zNeg = (height == cubes[x][z - 1].height);
			if (z < chunkSize - 1)
				zPos = (height == cubes[x][z + 1].height);
		}
	}

	renderer->setUpBuffers(vertices, indices);
}

void Chunk::generateCubeData(int x, int y, int z)
{
	const bool shouldRenderFace[6] = { !xPos, !xNeg, !yPos, yNeg, !zPos, !zNeg };

	int baseIndex = vertices.size();
	for (int face = 0; face < 6; ++face) {
		if (shouldRenderFace[face]) {

			for (int i = 0; i < 4; ++i) {
				glm::vec3 position(
					x + Cube::faceVertices[face][i][0],
					y + Cube::faceVertices[face][i][1],
					z + Cube::faceVertices[face][i][2]
				);
				glm::vec2 uvCoord = Cube::uv[face * 4 + i];
				vertices.push_back({ position, Cube::normals[face], uvCoord });
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

void Chunk::start()
{
	//generateNoise(0, 0, chunkSize, cubes);
}

void::Chunk::update(float deltaTime)
{
	renderer->draw();
}