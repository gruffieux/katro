#ifndef CBALL
#define CBALL

#include <cvisual.h>

class Ball : public Visual
{
private:
	int playerNb;
public:
	Ball();
	Ball(int playerNb);
	Ball(Ball &model);
	~Ball();
	static Ball* getBallElement(int index, List *pList) {return dynamic_cast<Ball*>(pList->GetElement(index));}
	int getPlayerNb() {return playerNb;}
	void initAni();
	void runAnimations();
};

#endif
