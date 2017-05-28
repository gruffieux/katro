#include <cinteger.h>
#include "cboard.h"
#include "../KaRes/resource.h"

Board::Board()
{
	playerNb = 0;
}

Board::Board(int playerNb)
{
	this->playerNb = playerNb;
}

Board::Board(Board &model)
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iter(model.getHoles());

	playerNb = model.getPlayerNb();

	hole = iter.first();

	while (hole)
	{
		holes.append(new Hole(*hole));
		hole = iter.next();
	}
}

Board::~Board()
{
}

int Board::ballsAmount()
{
	int amount;
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(&holes);

	amount = 0;
	hole = iterator.first();

	while (hole)
	{
		amount += hole->getBalls()->GetElementCount();
		hole = iterator.next();
	}

	return amount;
}

void Board::destroy()
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(&holes);

	hole = iterator.first();

	while (hole)
	{
		hole->clear();
		hole->destroyAni();
		hole = iterator.next();
	}

	holes.clear();
}

void Board::destroyAni()
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(&holes);

	hole = iterator.first();

	while (hole)
	{
		hole->destroyAni();
		hole = iterator.next();
	}

	Visual::destroyAni();
}

bool Board::init(int holesTotal)
{
	int i;
	Hole *hole;

	if (!Integer(holesTotal).IsDivisable(2))
		return false;

	dimension.x = 128 * holesTotal / 2;
	dimension.y = 128 * 2;

	setMosaic(true);

	for (i = 0; i < holesTotal; i++)
	{
		hole = new Hole(playerNb);
		hole->link(&holes);
	}

	return update(holesTotal);
}

void Board::initAni()
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(&holes);

	hole = iterator.first();

	while (hole)
	{
		hole->initAni();
		hole = iterator.next();
	}

	switch (playerNb)
	{
	case 1:
		addAni(IDB_BOARD1, "", 1, 128, 128, -1, 90);
		break;
	case 2:
		addAni(IDB_BOARD2, "", 1, 128, 128, -1, 90);
		break;
	default:
		break;
	}
}

void Board::restoreAni()
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(&holes);

	hole = iterator.first();

	while (hole)
	{
		hole->restoreAni();
		hole = iterator.next();
	}

	Visual::restoreAni();
}

void Board::runAnimations()
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(&holes);

	setVisibleZone();
	runAni(0, 0);

	hole = iterator.first();

	while (hole)
	{
		hole->runAnimations();
		hole = iterator.next();
	}
}

bool Board::update(int holesTotal)
{
	int i, x, y;
	Direction direction;
	Hole::Rank rank;
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(&holes);

	if (!Integer(holesTotal).IsDivisable(2))
		return false;

	i = x = 0;
	y = 128;
	direction = LeftToRight;
	rank = Hole::BACK;

	hole = iterator.first();

	while (hole)
	{
		if (i == holesTotal / 2 - 1)
			direction = DownToUp;

		if (i == holesTotal / 2)
		{
			y = 0;
			direction = RightToLeft;
			rank = Hole::FRONT;
		}

		hole->setRank(rank);
		hole->getPosition()->x = position.x + x;
		hole->getPosition()->y = position.y + y;
		hole->getLayers()->RemoveAllElement(0, true);
		hole->layBalls();

		switch (direction)
		{
		case LeftToRight:
			x += 128;
			break;
		case RightToLeft:
			x -= 128;
			break;
		default:
			break;
		}

		hole = iterator.next();
		i++;
	}

	return true;
}
