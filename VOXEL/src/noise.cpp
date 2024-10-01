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

            // Use or store the noise value (here we print it)
            //std::cout << std::floor(((noiseValue + 1) / 2) * chunkSize) << " ";
            
            blocks[x][(int)std::floor(((noiseValue + 1) / 2) * chunkSize)][z].setActive(true);
        }
        //std::cout << std::endl;
    }
}

//
//// Configuration variables
//float noiseScale = 0.05f;  // Scale for the noise (frequency)
//int chunkSize = 16;        // Size of the chunk
//
//// Function to generate noise for a chunk
//void generateChunk(int chunkX, int chunkY, int chunkSize, int worldSeed) {
//    // Create a FastNoiseLite instance
//    FastNoiseLite noiseGenerator;
//
//
//    // Set the seed for deterministic noise generation
//    noiseGenerator.SetSeed(worldSeed);
//
//    // Set the noise type to Perlin (you can change to others like Simplex)
//    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
//
//    // Optional: You can configure other settings like frequency
//    noiseGenerator.SetFrequency(noiseScale);  // Controls the size of features
//
//    // Generate noise for the chunk
//    
//}
