#pragma once
#include "cobject.h"
#include "chole.h"
#include <clinkedlist.h>

class Node : public Object
{
public:
	Node(int, int);
	~Node();
	void init(LinkedList<Hole*>*, bool player);
	void simulate(int, int, bool);
private:
	int width, height, min, max;
	int **board, **playerBoard;
};

