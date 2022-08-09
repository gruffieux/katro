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
	void setLevel(int level) {this->level = level;}
	void think(int holes);
	static const int INFINITY = 999;
private:
	int level;
	int minimax(Node* node, int holes, int depth, bool player, int alpha, int beta);
};

#endif