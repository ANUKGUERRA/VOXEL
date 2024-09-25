#ifndef BLOCK_H
#define BLOCK_H

class Block{

public:
	Block();
	~Block();
	bool isActive();
	void setActive(bool active);
private:
	bool active;
};
#endif