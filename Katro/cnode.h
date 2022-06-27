#pragma once
#include "cobject.h"
#include "chole.h"
#include <clinkedlist.h>

class Node : public Object
{
public:
	Node(Hole*, int, int);
	~Node();
	void init(LinkedList<Hole*>*, bool player);
	void simulate(int, int, bool);
	int getMin() { return min; }
	int getMax() { return max; }
	Hole* getFocus() { return focus; }
private:
	int width, height, min, max;
	int **board, **playerBoard;
	Hole* focus;
};

