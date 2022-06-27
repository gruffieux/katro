#pragma once
#include "cnode.h"

class NodeList : public List
{
public:
	static const int ORDER_BY_MIN = 4;
	static const int ORDER_BY_MAX = 5;
	NodeList();
	NodeList(int order);
	NodeList(NodeList& model);
	~NodeList();
	Node* getNodeElement(int index) { return dynamic_cast<Node*>(GetElement(index)); }
	int OrderBy(int NewOrder, Container* pContainer = NULL);
};

