#include "cball.h"
#include "../KaRes/resource.h"

Ball::Ball()
{
	playerNb = 0;
}

Ball::Ball(int playerNb) : Item(0, 0, 23, 24, Str())
{
	this->playerNb = playerNb;
}

Ball::Ball(Ball &model) : Item(model)
{
	playerNb = model.getPlayerNb();
}

Ball::~Ball()
{
}

void Ball::initAni()
{
	addAni(IDB_BALLS, "", 2, dimension.x, dimension.y, RGB(255, 255, 255), 90);
}

void Ball::runAnimations()
{
	setVisibleZone();
	runAni(0, playerNb - 1);
}
