#ifndef CPLAYER
#define CPLAYER

#include <cobject.h>
#include "cboard.h"

class Player : public Object
{
public:
	enum Action
	{
		NO_ACTION,
		TURN_START,
		TURN_ABORT,
		DISPLACE_MOVE,
		DISPLACE_CONTINUE,
		PICKUP_START,
		PICKUP_GO,
		PICKUP_BACK,
		TURN_END
	};

	Player();
	Player(int number, Str name);
	~Player();

	LinkedList<int>* getActions() {return &actions;}
	Board* getBoard() {return board;}
	Hole* getFocus() {return focus;}
	Mover* getHand() {return hand;}
	int getNumber() {return number;}
	Player* getOpponent() {return opponent;}
	Action playTurn();
	void prepareBoard();
	void prepareTestBoard(int *tab);
	void setFocus(Hole *focus) {this->focus = focus;}
	void setHand(Mover *hand) {this->hand = hand;}
	void setOpponent(Player *opponent) {this->opponent = opponent;}
	void startTurn(Board::Direction direction);
	bool updateFocus();

	static bool access;
	static const int MIN_MAGNITUDE = 3;
	static const int MAX_MAGNITUDE = 60;
protected:
	int number;
	Board *board;
	Hole *focus;
	Player *opponent;
	void abortTurn();
	bool displaceBalls();
	bool displaceBallsContinue();
	bool displaceBallsStart();
	void endTurn();
	bool insertAction(Action action, bool next);
	void nextAction();
	bool pickupBallsBack();
	bool pickupBallsGo();
	bool pickupBallsStart();
	bool releaseAllBalls();
	bool releaseBalls(int n);
	bool takeBalls(Hole *hole);
private:
	double currentMagnitude;
	Board::Direction currentDirX, currentDirY;
	Mover *hand;
	Hole *src1, *src2, *dest;
	List ballsInHand;
	LinkedList<int> actions;
	LinkedList<int>::Iterator *actionsIter;
};

#endif