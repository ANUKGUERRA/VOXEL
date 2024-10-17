#ifndef MAP_H
#define MAP_H
#include <unordered_map>
#include "../../GameObjects/Chunk.h"

struct vec2
{
	int x;
	int z;
	vec2() : x(x), z(z) {}
	vec2(int x, int z) : x(x), z(z) {}
};

class Map
{
public:
	Map();
	~Map();

	void updateMap(float camX, float camZ);
	//std::vector<ColliderComponent*> getPotentialCollisions(const ColliderComponent& playerCollider);
	//std::vector<ColliderComponent*> potentialCollisions;
	void initializeMap();

private:
	void loadNewChunks(int dx, int dz);
	void removeOldChunks(int dx, int dz);
	//void drawAreaChunks(Application& app);
	int nChunks = 5;
	vec2 currentChunk = { 0,0 };
	
	std::unordered_map<int, Chunk*> chunks;

};
#endif
