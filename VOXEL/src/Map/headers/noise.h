#pragma once
#include "FastNoiseLite.h"
#include "../../GameObjects/Chunk.h"

class Chunk;

void generateNoise(int chunkX, int chunkY, int chunkSize, Cube **blocks);