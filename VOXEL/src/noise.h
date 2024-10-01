#pragma once
#include "FastNoiseLite.h"

//Circular dependency (Forward Declaration)
class Chunk;

void generateNoise(int chunkX, int chunkY, int chunkSize, Chunk &Chunk);