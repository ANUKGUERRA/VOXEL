#include "headers/Map.h"
#include <cmath>
#include "../GameObjects/Chunk.h"
#include "headers/Cantor.h"
#include "../Application.h"


Map::Map() : currentChunk({ 0, 0 }) {
    initializeMap();
}

Map::~Map() {
    for (auto& pair : chunks) {
        delete pair.second;
    }
}

void Map::initializeMap() {
    for (int x = -nChunks; x <= nChunks; ++x) {
        for (int z = -nChunks; z <= nChunks; ++z) {
            int key = cantorPair(currentChunk.x + x, currentChunk.z + z);
            chunks[key] = new Chunk(currentChunk.x + x, currentChunk.z + z);
        }
    }
}

void Map::updateMap(float playerX, float playerZ) {
    int newChunkX = floor(playerX / Chunk::chunkSize);
    int newChunkZ = floor(playerZ / Chunk::chunkSize);

    if (currentChunk.x != newChunkX || currentChunk.z != newChunkZ) {
        int dx = newChunkX - currentChunk.x;
        int dz = newChunkZ - currentChunk.z;
        currentChunk = { newChunkX, newChunkZ };
        loadNewChunks(dx, dz);
        removeOldChunks(dx, dz);
    }
}

void Map::loadNewChunks(int dx, int dz) {
    int startX = (dx > 0) ? currentChunk.x + nChunks - dx + 1 : currentChunk.x - nChunks;
    int endX = (dx < 0) ? currentChunk.x + nChunks : currentChunk.x + nChunks + 1;
    int startZ = (dz > 0) ? currentChunk.z + nChunks - dz + 1 : currentChunk.z - nChunks;
    int endZ = (dz < 0) ? currentChunk.z + nChunks : currentChunk.z + nChunks + 1;

    for (int x = startX; x <= endX; ++x) {
        for (int z = startZ; z <= endZ; ++z) {
            int key = cantorPair(x, z);
            if (chunks.find(key) == chunks.end()) {
                chunks[key] = new Chunk(x, z);
            }
        }
    }
}

void Map::removeOldChunks(int dx, int dz) {
    int removeStartX = (dx > 0) ? currentChunk.x - nChunks : currentChunk.x + nChunks - dx + 1;
    int removeEndX = (dx < 0) ? currentChunk.x + nChunks : currentChunk.x - nChunks + 1;
    int removeStartZ = (dz > 0) ? currentChunk.z - nChunks : currentChunk.z + nChunks - dz + 1;
    int removeEndZ = (dz < 0) ? currentChunk.z + nChunks : currentChunk.z - nChunks + 1;

    std::vector<int> keysToRemove;
    for (int x = removeStartX; x <= removeEndX; ++x) {
        for (int z = removeStartZ; z <= removeEndZ; ++z) {
            int key = cantorPair(x, z);
            if (chunks.find(key) != chunks.end()) {
                keysToRemove.push_back(key);
            }
        }
    }

    for (int key : keysToRemove) {
        delete chunks[key];
        chunks.erase(key);
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

//std::vector<Collider*> Map::getPotentialCollisions(const Collider& playerCollider) {
//
//    glm::vec3 minSearch = playerCollider.min - glm::vec3(1, 1, 1);
//    glm::vec3 maxSearch = playerCollider.max + glm::vec3(1, 1, 1);
//
//    int minChunkX = floor(minSearch.x / Chunk::chunkSize);
//    int maxChunkX = ceil(maxSearch.x / Chunk::chunkSize);
//    int minChunkZ = floor(minSearch.z / Chunk::chunkSize);
//    int maxChunkZ = ceil(maxSearch.z / Chunk::chunkSize);
//
//    for (int chunkX = minChunkX; chunkX <= maxChunkX; chunkX++) {
//        for (int chunkZ = minChunkZ; chunkZ <= maxChunkZ; chunkZ++) {
//            int key = cantorPair(chunkX, chunkZ);
//            if (chunks.find(key) != chunks.end()) {
//                Chunk* chunk = chunks[key];
//
//
//                int startX = floor(minSearch.x) - chunkX * Chunk::chunkSize;
//                int endX = ceil(maxSearch.x) - chunkX * Chunk::chunkSize;
//                int startZ = floor(minSearch.z) - chunkZ * Chunk::chunkSize;
//                int endZ = ceil(maxSearch.z) - chunkZ * Chunk::chunkSize;
//
//
//                startX = std::max(0, std::min(startX, Chunk::chunkSize - 1));
//                endX = std::max(0, std::min(endX, Chunk::chunkSize - 1));
//                startZ = std::max(0, std::min(startZ, Chunk::chunkSize - 1));
//                endZ = std::max(0, std::min(endZ, Chunk::chunkSize - 1));
//
//                for (int x = startX; x <= endX; x++) {
//                    for (int z = startZ; z <= endZ; z++) {
//                        int worldY = chunk->blocks[x][z].height;
//                        if (worldY >= minSearch.y && worldY <= maxSearch.y) {
//                            potentialCollisions.push_back(&chunk->blocks[x][z].collider);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return potentialCollisions;
//}