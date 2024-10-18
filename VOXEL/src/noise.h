#pragma once
#include <FastNoiseLite.h>
#include "Chunk.h"

//Circular dependency (Forward Declaration)
class Chunk;

void generateNoise(int chunkX, int chunkY, int chunkSize, Cube **blocks);