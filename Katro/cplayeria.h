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
	//bool attack(HoleList holes);
	//bool defend(HoleList holes);
	//void despair();
	void setLevel(int level) {this->level = level;}
	//void think();
	void think(int holes);
protected:
	//Hole* findStartHole(Hole *end);
	//void lookBoard(HoleList *frontHoles, Hole **preciousHole);
	//void lookOpBoard(HoleList *frontBalls);
private:
	int level;
	int minimax(Node* node, int holes, int depth, bool player, int alpha, int beta);
	//bool searchStartHole(LinkedList<Hole*>::Iterator vBoardIter, LinkedList<Hole*>::Iterator targetBoardIter, int distance, int maxStep, int step);
};

#endif
