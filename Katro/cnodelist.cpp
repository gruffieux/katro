#include "cnodelist.h"

NodeList::NodeList()
{
}

NodeList::NodeList(int order) : List(order)
{
}

NodeList::NodeList(NodeList& model) : List(model)
{
}

NodeList::~NodeList()
{
}

int NodeList::OrderBy(int NewOrder, Container* pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_MIN:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = getNodeElement(i)->getMin();
			result = OrderElements(ORDER_NUM);
			if (!result)
				return result;
			break;
		case ORDER_BY_MAX:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = getNodeElement(i)->getMax();
			result = OrderElements(ORDER_NUM);
			if (!result)
				return result;
			break;
		default:
			return ORDER_UNKNOWN;
		}
	}
	else
		return result;

	FinalizeOrder(NewOrder);

	return result;
}
