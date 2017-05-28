#ifndef CHOLELIST
#define CHOLELIST

#include "chole.h"

class HoleList : public List
{
public:
	static const int ORDER_BY_BALLS = 4;
	HoleList();
	HoleList(int order);
	HoleList(HoleList &model);
	~HoleList();
	Hole* getHoleElement(int index) {return dynamic_cast<Hole*>(GetElement(index));}
	int OrderBy(int NewOrder, Container *pContainer=NULL);
};

#endif
