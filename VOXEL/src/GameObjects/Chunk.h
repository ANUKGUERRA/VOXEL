#pragma once
#include "../Components/headers/ColliderComponent.h"
#include "../Components/headers/RendererComponent.h"
#include "../Map/headers/noise.h"
#include "Cube.h"
class Chunk : public GameObject
{
public:

	Chunk(int chunkX, int chunkY);
	~Chunk();
	RendererComponent *renderer;

	void generateChunk(int chunkX, int chunkZ);
	void generateCubeData(int x, int y, int z);

	void start() override;
	void update(float deltaTime) override;

	Cube **cubes;
	static const int chunkSize = 64;
	bool xNeg, xPos, yNeg, yPos, zNeg, zPos;
	int chunkX, chunkZ;

	std::vector<RendererComponent::Vertex> vertices;
	std::vector<int> indices;
	
};

