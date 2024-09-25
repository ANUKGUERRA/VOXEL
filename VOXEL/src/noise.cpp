#include <iostream>
#include "FastNoiseLite.h"  // Include FastNoiseLite header

// Configuration variables
float noiseScale = 0.05f;  // Scale for the noise (frequency)
int chunkSize = 16;        // Size of the chunk

// Function to generate noise for a chunk
void generateChunk(int chunkX, int chunkY, int chunkSize, int worldSeed) {
    // Create a FastNoiseLite instance
    FastNoiseLite noiseGenerator;

    // Set the seed for deterministic noise generation
    noiseGenerator.SetSeed(worldSeed);

    // Set the noise type to Perlin (you can change to others like Simplex)
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    // Optional: You can configure other settings like frequency
    noiseGenerator.SetFrequency(noiseScale);  // Controls the size of features

    // Generate noise for the chunk
    for (int y = 0; y < chunkSize; y++) {
        for (int x = 0; x < chunkSize; x++) {
            // Calculate global coordinates for noise
            float globalX = (chunkX * chunkSize) + x;
            float globalY = (chunkY * chunkSize) + y;

            // Get noise value at the global coordinates
            float noiseValue = noiseGenerator.GetNoise(globalX, globalY);

            // Use or store the noise value (here we print it)
            std::cout << noiseValue << " ";
        }
        std::cout << std::endl;
    }
}
