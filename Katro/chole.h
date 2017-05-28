#ifndef CHOLE
#define CHOLE

#include <cvisual.h>
#include <clinkedlist.h>

class Hole : public Visual
{
public:
	enum Rank
	{
		FRONT,
		BACK
	};
	Hole();
	Hole(int playerNb);
	Hole(Hole &model);
	~Hole();
	bool addBalls(List *pList);
	void clear();
	void destroyAni();
	List* getBalls() {return &balls;}
	List* getLayers() {return &layers;}
	Hole* getNext() {return next;}
	Hole* getOpposite() {return opposite;}
	int getPlayerNb() {return playerNb;}
	Hole* getPrevious() {return previous;}
	Rank getRank() {return rank;}
	void initAni();
	void layBalls();
	void link(LinkedList<Hole*> *holes);
	void removeBalls();
	void runAnimations();
	void restoreAni();
	void setNext(Hole *next) {this->next = next;}
	void setOpposite(Hole *opposite) {this->opposite = opposite;}
	void setPrevious(Hole *previous) {this->previous = previous;}
	void setRank(Rank rank) {this->rank = rank;}
	void updateIndicator();
private:
	int playerNb;
	Rank rank;
	Surface indicator;
	Hole *next, *previous, *opposite;
	List balls, layers;
};

#endif
