#pragma once
#include "FastNoiseLite.h"
#include "../../GameObjects/Chunk.h"

class Cube;

void generateNoise(int chunkX, int chunkY, int chunkSize, Cube **blocks);