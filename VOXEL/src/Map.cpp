#include "Map.h"
#include <cmath>
#include "Chunk.h"
#include "Cantor.h"
#include "Application.h"


Map::Map()
{

}

Map::~Map()
{

}

void Map::loadMap()
{
    loadAreaChunks();
}

void Map::updateMap(float camX, float camZ, Application& app)
{
    camX = floor(camX / Chunk::chunkSize);
    camZ = floor(camZ / Chunk::chunkSize);

    

    if (currentChunk.x != camX || currentChunk.z != camZ)
    {
        currentChunk = { camX, camZ };
        loadAreaChunks();
    }

    drawAreaChunks(app);

};

void Map::loadAreaChunks()
{
    for (int x = -nChunks; x <= nChunks; x++)
    {
        for (int z = -nChunks; z <= nChunks; z++)
        {
            int key = cantorPair(currentChunk.x + x, currentChunk.z + z);

            if (chunks.find(key) == chunks.end())
            {
                Chunk* chunk = new Chunk();
                chunks[key] = chunk;
                chunks[key]->init(currentChunk.x + x, currentChunk.z + z);
            }
        }
    }
}
void Map::drawAreaChunks(Application& app)
{
    for (int x = -nChunks; x <= nChunks; x++)
    {
        for (int z = -nChunks; z <= nChunks; z++)
        {
            int key = cantorPair(currentChunk.x + x, currentChunk.z + z);
            app.renderMap(chunks[key]);
        }
    }
}

int encode(int v) {
    return v >= 0 ? 2 * v : -2 * v - 1;
}

int decode(int v) {
    return v % 2 == 0 ? v / 2 : -(v / 2) - 1;
}

int triangularNumber(int sum) {
    return (sum * (sum + 1)) / 2;
}

int cantorPair(int x, int z) {
    int encodedX = encode(x);
    int encodedZ = encode(z);
    int sum = encodedX + encodedZ;
    return triangularNumber(sum) + encodedZ;
}

vec2 cantorUnpair(int pair) {
    int sum = floor((sqrt(8 * pair + 1) - 1) / 2);
    int triangularSum = (sum * (sum + 1)) / 2;
    int encodedZ = pair - triangularSum;
    int encodedX = sum - encodedZ;

    int x = decode(encodedX);
    int z = decode(encodedZ);

    return vec2(x, z);
}

std::vector<Collider*> Map::getPotentialCollisions(const Collider& playerCollider) {

    glm::vec3 minSearch = playerCollider.min - glm::vec3(1, 1, 1);
    glm::vec3 maxSearch = playerCollider.max + glm::vec3(1, 1, 1);

    int minChunkX = floor(minSearch.x / Chunk::chunkSize);
    int maxChunkX = ceil(maxSearch.x / Chunk::chunkSize);
    int minChunkZ = floor(minSearch.z / Chunk::chunkSize);
    int maxChunkZ = ceil(maxSearch.z / Chunk::chunkSize);

    for (int chunkX = minChunkX; chunkX <= maxChunkX; chunkX++) {
        for (int chunkZ = minChunkZ; chunkZ <= maxChunkZ; chunkZ++) {
            int key = cantorPair(chunkX, chunkZ);
            if (chunks.find(key) != chunks.end()) {
                Chunk* chunk = chunks[key];


                int startX = floor(minSearch.x) - chunkX * Chunk::chunkSize;
                int endX = ceil(maxSearch.x) - chunkX * Chunk::chunkSize;
                int startZ = floor(minSearch.z) - chunkZ * Chunk::chunkSize;
                int endZ = ceil(maxSearch.z) - chunkZ * Chunk::chunkSize;


                startX = std::max(0, std::min(startX, Chunk::chunkSize - 1));
                endX = std::max(0, std::min(endX, Chunk::chunkSize - 1));
                startZ = std::max(0, std::min(startZ, Chunk::chunkSize - 1));
                endZ = std::max(0, std::min(endZ, Chunk::chunkSize - 1));

                for (int x = startX; x <= endX; x++) {
                    for (int z = startZ; z <= endZ; z++) {
                        int worldY = chunk->blocks[x][z].height;
                        if (worldY >= minSearch.y && worldY <= maxSearch.y) {
                            potentialCollisions.push_back(&chunk->blocks[x][z].collider);
                        }
                    }
                }
            }
        }
    }
    return potentialCollisions;
}