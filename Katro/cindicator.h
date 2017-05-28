#ifndef CINDICATOR
#define CINDICATOR

#include <cpanel.h>

class Indicator : public Panel
{
private:
	Surface text;
public:
	Indicator();
	Indicator(Str content);
	~Indicator();
	void destroyAni();
	static Indicator* getIndicatorElement(int index, List *pList) {return dynamic_cast<Indicator*>(pList->GetElement(index));}
	void Init();
	void initAni();
	bool Refresh();
	void restoreAni();
	void runAnimations();
	void updateContent();
};

#endif
