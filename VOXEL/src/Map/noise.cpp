#include "headers/noise.h"
#include <iostream>
void generateNoise(int chunkX, int chunkZ, int chunkSize, Cube **blocks)
{
    FastNoiseLite fastNoise;
    fastNoise.SetSeed(1);
    fastNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    

    for (int x = 0; x < chunkSize; x++) {
        for (int z = 0; z < chunkSize; z++) {


            float globalX = (chunkX * chunkSize) + x;
            float globalZ = (chunkZ * chunkSize) + z;


            float noiseValue = fastNoise.GetNoise(globalX, globalZ);
            
            blocks[x][z].height = (int)std::floor(((noiseValue + 1) / 2) * chunkSize);
        }
    }
}
