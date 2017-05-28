#include "cholelist.h"

HoleList::HoleList()
{
}

HoleList::HoleList(int order) : List(order)
{
}

HoleList::HoleList(HoleList &model) : List(model)
{
}

HoleList::~HoleList()
{
}

int HoleList::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_BALLS:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = getHoleElement(i)->getBalls()->GetElementCount();
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
