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
	bool filterHighestScore(int delta);
	int OrderBy(int NewOrder, Container* pContainer = NULL);
};

class Node : public Object
{
public:
	Node();
	Node(int, int, Hole*);
	~Node();
	void init(LinkedList<Hole*> *holes, bool player);
	void init(int **board, int **playerBoard);
	bool simulate(int index, bool player, int maxRounds);
	int** getBoard() { return board; }
	int** getPlayerBoard() { return playerBoard; }
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

