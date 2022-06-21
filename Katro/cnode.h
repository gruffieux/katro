#pragma once
#include "cobject.h"
class Node : public Object
{
public:
	Node(int, int, int);
	~Node();
	int simulate(int, int);
private:
	int width, height, ballsPerHole;
	int **board, **playerBoard;
};

