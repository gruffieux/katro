#ifndef CBOARD
#define CBOARD

#include <clinkedlist.h>
#include <ccursor.h>
#include "chole.h"

class Board : public Visual
{
private:
	int playerNb;
	LinkedList<Hole*> holes;
public:
	enum Direction
	{
		LeftToRight,
		RightToLeft,
		UpToDown,
		DownToUp
	};

	Board();
	Board(int playerNb);
	Board(Board &model);
	~Board();
	int ballsAmount();
	void destroy();
	void destroyAni();
	LinkedList<Hole*>* getHoles() {return &holes;}
	int getPlayerNb() {return playerNb;}
	bool init(int holesTotal);
	void initAni();
	void restoreAni();
	void runAnimations();
	bool update(int holesTotal);
};

#endif