#pragma once
#include "chole.h"
#include <clinkedlist.h>

class NodeList : public List
{
public:
	static const int ORDER_BY_SCORE = 4;
	NodeList();
	NodeList(int order);
	NodeList(NodeList& model);
	~NodeList();
	int OrderBy(int NewOrder, Container* pContainer = NULL);
};

class Node : public Object
{
public:
	Node() { ; }
	Node(int, int, Hole*);
	~Node();
	void init(LinkedList<Hole*>*, bool player);
	void simulate(int, bool);
	Hole* getFocus() { return focus; }
	static Node* getNodeElement(List* pList, int index) { return dynamic_cast<Node*>(pList->GetElement(index)); }
	int getScore() { return score; }
	NodeList* getChilds() { return &childs; }
	bool hasChild() { return childs.GetElementCount() > 0; }
private:
	int width, height, score;
	int **board, **playerBoard;
	Hole* focus;
	NodeList childs;
};

