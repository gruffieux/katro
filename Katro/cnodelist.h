#pragma once
#include "chole.h"
#include <clinkedlist.h>

class NodeList : public List
{
public:
	static const int ORDER_BY_MIN = 4;
	static const int ORDER_BY_MAX = 5;
	NodeList();
	NodeList(int order);
	NodeList(NodeList& model);
	~NodeList();
	int OrderBy(int NewOrder, Container* pContainer = NULL);
};

class Node : public Object
{
public:
	Node(int, int, Hole*);
	~Node();
	void init(LinkedList<Hole*>*, bool player);
	void simulate(int, bool);
	int getMin() { return min; }
	int getMax() { return max; }
	Hole* getFocus() { return focus; }
	static Node* getNodeElement(List* pList, int index) { return dynamic_cast<Node*>(pList->GetElement(index)); }
private:
	int width, height, min, max;
	int **board, **playerBoard;
	Hole* focus;
	NodeList childs;
};

