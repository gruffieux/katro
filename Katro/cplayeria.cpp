#include "cplayeria.h"
#include "cball.h"

PlayerIA::PlayerIA(Str name) : Player(2, name)
{
	level = 2;
}

PlayerIA::~PlayerIA()
{
}

bool PlayerIA::attack(HoleList holes)
{
	int i;
	Hole *hole = NULL;

	for (i = 0; i < holes.GetElementCount(); i++)
	{
		hole = findStartHole(holes.getHoleElement(i));
		if (hole)
		{
			focus = hole;
			return true;
		}
	}

	return false;
}

bool PlayerIA::defend(HoleList holes)
{
	int i;
	Hole *hole;

	for (i = 0; i < holes.GetElementCount(); i++)
	{
		hole = holes.getHoleElement(i);
		if (findStartHole(hole))
		{
			focus = hole;
			return true;
		}
	}

	return false;
}

void PlayerIA::despair()
{
	int i;
	Hole *hole;
	HoleList holes;
	LinkedList<Hole*>::Iterator iter(board->getHoles());

	hole = iter.first();

	while (hole)
	{
		if (hole->getBalls()->GetElementCount())
			holes.AddElement(hole);
		hole = iter.next();
	}

	if (holes.GetElementCount())
	{
		i = Clock::random(0, holes.GetElementCount()-1);
		focus = holes.getHoleElement(i);
	}
	else
		focus = NULL;
}

Hole* PlayerIA::findStartHole(Hole *end)
{
	int i, j, distance, maxDistance;
	Hole *start;
	Board *vBoard, *targetBoard;
	LinkedList<Hole*>::Iterator iter[3];

	if (!end->getOpposite()->getBalls()->GetElementCount())
		return NULL;

	if (end->getPlayerNb() == number)
		targetBoard = opponent->getBoard();
	else
		targetBoard = board;

	maxDistance = 0;

	iter[0].setList(targetBoard->getHoles());
	
	if (!iter[0].find(end->getOpposite()))
		return NULL;

	while (iter[0].hasNext())
	{
		maxDistance++;
		iter[0].next();
	}

	i = 0;
	start = NULL;
	iter[0].last();

	while (iter[0].hasPrevious() && !start)
	{
		distance = maxDistance;
		vBoard = new Board(*targetBoard);
		iter[1].setList(vBoard->getHoles());
		iter[2].setList(targetBoard->getHoles());
		iter[1].last();
		iter[2].last();
		for (j = 0; j < i; j++)
		{
			iter[1].previous();
			iter[2].previous();
			distance--;
		}
		if (searchStartHole(iter[1], iter[2], distance, level, 0))
			start = iter[2].currentData();
		iter[0].previous();
		i++;
		vBoard->destroy();
		delete vBoard;
	}

	return start;
}

void PlayerIA::lookBoard(HoleList *frontHoles, Hole **preciousHole)
{
	int i;
	Hole *hole, *unsafeHole;
	LinkedList<Hole*>::Iterator iter(board->getHoles());

	hole = iter.first();

	while (hole)
	{
		if (hole->getRank() == Hole::BACK && hole->getBalls()->GetElementCount())
			frontHoles->AddElement(hole);
		hole = iter.next();
	}

	unsafeHole = NULL;

	for (i = 0; i < frontHoles->GetElementCount(); i++)
	{
		hole = frontHoles->getHoleElement(i);
		if (hole->getBalls()->GetElementCount() >= 5 && findStartHole(hole))
			if (!unsafeHole || hole->getBalls()->GetElementCount() > unsafeHole->getBalls()->GetElementCount())
				unsafeHole = hole;
	}

	*preciousHole = unsafeHole;
}

void PlayerIA::lookOpBoard(HoleList *frontBalls)
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iter(opponent->getBoard()->getHoles());

	hole = iter.first();

	while (hole)
	{
		if (hole->getRank() == Hole::FRONT && hole->getBalls()->GetElementCount())
			frontBalls->AddElement(hole);
		hole = iter.next();
	}
}

bool PlayerIA::searchStartHole(LinkedList<Hole*>::Iterator vBoardIter, LinkedList<Hole*>::Iterator targetBoardIter, int distance, int maxStep, int step)
{
	int i, n, d;
	Hole *hole;

	hole = vBoardIter.currentData();

	if (!hole)
		return false;

	n = hole->getBalls()->GetElementCount();

	if (!n)
		return false;

	if (n >= board->getHoles()->elementCount())
		return false;

	if (!distance)
		return false;

	if (n == distance)
		return true;

	step++;

	if (step >= maxStep)
		return false;

	hole->getBalls()->RemoveAllElement(0, true);

	for (i = 0; i < n; i++)
	{
		if (vBoardIter.hasPrevious())
		{
			hole = vBoardIter.previous();
			targetBoardIter.previous();
		}
		else
		{
			hole = vBoardIter.last();
			targetBoardIter.last();
		}
		hole->getBalls()->AddElement(new Ball());
	}

	if (hole->getBalls()->GetElementCount() <= 1)
		return false;

	d = distance - n;

	if (d < 0)
		distance = board->getHoles()->elementCount() + d;
	else
		distance = d;

	return searchStartHole(vBoardIter, targetBoardIter, distance, maxStep, step);
}

void PlayerIA::think()
{
	Hole *preciousHole = NULL;
	HoleList frontHoles(HoleList::ORDER_BY_BALLS), opFrontHoles(HoleList::ORDER_BY_BALLS);

	lookBoard(&frontHoles, &preciousHole);

	if (frontHoles.GetElementCount())
	{
		frontHoles.ReverseOrder();
		if (preciousHole)
			focus = preciousHole;
		else
		{
			lookOpBoard(&opFrontHoles);
			if (opFrontHoles.GetElementCount())
			{
				opFrontHoles.ReverseOrder();
				if (!attack(opFrontHoles) && !defend(frontHoles))
					despair();
			}
			else
				if (!defend(frontHoles))
					despair();
		}
	}
	else
		if (!defend(frontHoles))
			despair();
}
