#include <iostream>
#include "noise.h"
void generateNoise(int chunkX, int chunkZ, int chunkSize, Block ***blocks)
{


    FastNoiseLite fastNoise;
    fastNoise.SetSeed(1);
    fastNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    

    for (int x = 0; x < chunkSize; x++) {
        for (int z = 0; z < chunkSize; z++) {


            float globalX = (chunkX * chunkSize) + x;
            float globalZ = (chunkZ * chunkSize) + z;


            float noiseValue = fastNoise.GetNoise(globalX, globalZ);
            
            blocks[x][(int)std::floor(((noiseValue + 1) / 2) * chunkSize)][z].setActive(true);
        }
    }
}
