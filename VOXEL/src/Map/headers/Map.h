#pragma once
#include <unordered_map>
#include "Chunk.h"

class Application;
struct vec2
{
	float x;
	float z;
	vec2() : x(x), z(z) {}
	vec2(float x, float z) : x(x), z(z) {}
};

class Map
{
public:
	Map();
	~Map();

	void updateMap(float camX, float camZ, Application& app);
	void loadMap();
	std::vector<Collider*> getPotentialCollisions(const Collider& playerCollider);
	std::vector<Collider*> potentialCollisions;

private:
	void loadAreaChunks();
	void drawAreaChunks(Application& app);
	int nChunks = 5;
	vec2 currentChunk = { 0.f,0.f };
	
	std::unordered_map<int, Chunk*> chunks;

};
