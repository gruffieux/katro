#ifndef CPLAYERIA
#define CPLAYERIA

#include "cplayer.h"
#include "cholelist.h"
#include "cnodelist.h"

class PlayerIA : public Player
{
public:
	PlayerIA(Str name="");
	~PlayerIA();
	bool attack(HoleList holes);
	bool defend(HoleList holes);
	void despair();
	int minimax(Node *node, int depth, bool player, int alpha, int beta);
	void setLevel(int level) {this->level = level;}
	void think();
	void think(int);
protected:
	Hole* findStartHole(Hole *end);
	void lookBoard(HoleList *frontHoles, Hole **preciousHole);
	void lookOpBoard(HoleList *frontBalls);
private:
	int level, holes;
	bool searchStartHole(LinkedList<Hole*>::Iterator vBoardIter, LinkedList<Hole*>::Iterator targetBoardIter, int distance, int maxStep, int step);
};

#endif
