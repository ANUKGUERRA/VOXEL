#include "Block.h"

Block::Block()
{
	active = false;
}

Block::~Block()
{
}

bool Block::isActive()
{
	return active;
}

void Block::setActive(bool active)
{
	this->active = active;
}