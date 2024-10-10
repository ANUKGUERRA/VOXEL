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