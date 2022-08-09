#include <csound.h>
#include "cplayer.h"
#include "cball.h"
#include "cnodelist.h"
#include "main.h"

bool Player::access = true;
extern List sounds;

Player::Player()
{
	number = 1;
	currentMagnitude = 0;
	currentDirX = Board::LeftToRight;
	currentDirY = Board::UpToDown;
	board = new Board(number);
	hand = NULL;
	focus = src1 = src2 = dest = NULL;
	opponent = NULL;
	actionsIter = NULL;
}

Player::Player(int number, Str name) : Object(name)
{
	this->number = number;
	currentMagnitude = 0;
	currentDirX = Board::LeftToRight;
	currentDirY = Board::UpToDown;
	board = new Board(number);
	hand = NULL;
	focus = src1 = src2 = dest = NULL;
	opponent = NULL;
	actionsIter = NULL;
}

Player::~Player()
{
	delete board;
}

void Player::abortTurn()
{
	actions.clear();

	delete actionsIter;

	actionsIter = NULL;
}

bool Player::displaceBalls()
{
	int x1, x2, x3, y1, y2, y3;
	bool holeWasEmpty;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	if (!dest)
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	x1 = hand->getPosition()->x + hand->getDimension()->x / 2;
	x2 = dest->getPosition()->x + dest->getDimension()->x / 2;
	y1 = hand->getPosition()->y + hand->getDimension()->y / 2;
	y2 = dest->getPosition()->y + dest->getDimension()->y / 2;

	if (!hand->GetMagnitude())
	{
		hand->SetMagnitude(currentMagnitude);
		if (x1 < x2)
			hand->SetAngle(0);
		else if (x1 > x2)
			hand->SetAngle(PI);
		else if (y1 < y2)
			hand->SetAngle(PI / 2);
		else if (y1 > y2)
			hand->SetAngle(3 * PI / 2);
	}

	hand->IncreaseMagnitude(MAX_MAGNITUDE);
	hand->Move();
	currentMagnitude = hand->GetMagnitude();

	focus = dest;
	holeWasEmpty = !focus->getBalls()->GetElementCount();

	if (hand->GetDirection()->west)
	{
		x1 = hand->getPosition()->x + hand->getDimension()->x / 2;
		if (x1 <= x2)
		{
			releaseBalls(1);
			x3 = x2 - x1;
			hand->getPosition()->x += x3;
			hand->SetMagnitude(0);
			if (dest->getRank() == Hole::FRONT)
			{
				dest = dest->getNext();
				if (!dest)
					dest = iterator.first();
			}
			else
			{
				dest = dest->getPrevious();
				if (!dest)
					dest = iterator.last();
			}
		}
	}
	else if (hand->GetDirection()->east)
	{
		x1 = hand->getPosition()->x + hand->getDimension()->x / 2;
		if (x1 >= x2)
		{
			releaseBalls(1);
			x3 = x1 - x2;
			hand->getPosition()->x -= x3;
			hand->SetMagnitude(0);
			if (dest->getRank() == Hole::BACK)
			{
				dest = dest->getNext();
				if (!dest)
				{
					insertAction(TURN_ABORT, true);
					return false;
				}
			}
			else
			{
				dest = dest->getPrevious();
				if (!dest)
				{
					insertAction(TURN_ABORT, true);
					return false;
				}
			}
		}
	}
	else if (hand->GetDirection()->north)
	{
		y1 = hand->getPosition()->y + hand->getDimension()->y / 2;
		if (y1 <= y2)
		{
			releaseBalls(1);
			y3 = y2 - y1;
			hand->getPosition()->y += y3;
			hand->SetMagnitude(0);
			if (dest == iterator.last())
				dest = dest->getPrevious();
			else
				dest = dest->getNext();
		}
	}
	else if (hand->GetDirection()->south)
	{
		y1 = hand->getPosition()->y + hand->getDimension()->y / 2;
		if (y1 >= y2)
		{
			releaseBalls(1);
			y3 = y1 - y2;
			hand->getPosition()->y -= y3;
			hand->SetMagnitude(0);
			if (dest == iterator.first())
				dest = dest->getNext();
			else
				dest = dest->getPrevious();
		}
	}

	// Plus de billes en main
	if (!ballsInHand.GetElementCount())
	{
		// Le dernier trou était vide
		if (holeWasEmpty)
			insertAction(TURN_END, true);
		else
		{
			// Prend les billes en face
			if (focus->getOpposite())
			{
				insertAction(PICKUP_START, true);
				insertAction(DISPLACE_CONTINUE, false);
			}
			else
				insertAction(DISPLACE_CONTINUE, true);
		}
	}

	return true;
}

bool Player::displaceBallsContinue()
{
	if (src1->getRank() != focus->getRank())
	{
		if (currentDirX == Board::LeftToRight)
			currentDirX = Board::RightToLeft;
		else
			currentDirX = Board::LeftToRight;
	}

	return displaceBallsStart();
}

bool Player::displaceBallsStart()
{
	int x1, x2, x3;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	if (!iterator.find(focus))
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	if (!focus->getBalls()->GetElementCount())
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	x1 = hand->getPosition()->x + hand->getDimension()->x / 2;
	x2 = focus->getPosition()->x + focus->getDimension()->x / 2;
	x3 = x2 - x1;

	hand->getPosition()->x += x3;

	takeBalls(focus);

	switch (currentDirX)
	{
	case Board::LeftToRight:
		if (focus->getRank() == Hole::BACK)
		{
			dest = iterator.next();
			if (!dest)
			{
				insertAction(TURN_ABORT, true);
				return false;
			}
		}
		else
		{
			dest = iterator.previous();
			if (!dest)
			{
				insertAction(TURN_ABORT, true);
				return false;
			}
		}
		break;
	case Board::RightToLeft:
		if (focus->getRank() == Hole::FRONT)
		{
			dest = iterator.next();
			if (!dest)
				dest = iterator.first();
		}
		else
		{
			dest = iterator.previous();
			if (!dest)
				dest = iterator.last();
		}
		break;
	default:
		insertAction(TURN_ABORT, true);
		return false;
	}

	insertAction(DISPLACE_MOVE, true);

	return true;
}

void Player::endTurn()
{
	actions.clear();

	delete actionsIter;

	actionsIter = NULL;
}

bool Player::insertAction(Action action, bool next)
{
	if (!actionsIter)
		return false;

	if (actionsIter->hasCurrent())
		return actionsIter->insert(action, actionsIter->currentData(), next);

	actions.append(action);

	if (next)
		nextAction();

	return true;
}

void Player::nextAction()
{
	Action action;

	if (!actionsIter->hasCurrent())
		action = (Action)actionsIter->first();
	else
		action = (Action)actionsIter->next();
}

bool Player::pickupBallsBack()
{
	int y1, y2, y3;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	switch (currentDirY)
	{
	case Board::DownToUp:
		hand->SetAngle(3 * PI / 2);
		break;
	case Board::UpToDown:
		hand->SetAngle(PI / 2);
		break;
	default:
		insertAction(TURN_ABORT, true);
		return false;
	}

	hand->SetMagnitude(currentMagnitude);
	hand->Move();

	y1 = hand->getPosition()->y + hand->getDimension()->y / 2;
	y2 = dest->getPosition()->y + dest->getDimension()->y / 2;

	if (hand->GetDirection()->north)
	{
		if (y1 <= y2)
		{
			focus = dest;
			releaseAllBalls();
			y3 = y2 - y1;
			hand->getPosition()->y += y3;
			hand->SetMagnitude(0);
			if (!opponent->getBoard()->ballsAmount())
				insertAction(TURN_END, false);
			nextAction();
		}
	}
	else if (hand->GetDirection()->south)
	{
		if (y1 >= y2)
		{
			focus = dest;
			releaseAllBalls();
			y3 = y1 - y2;
			hand->getPosition()->y -= y3;
			hand->SetMagnitude(0);
			if (!opponent->getBoard()->ballsAmount())
				insertAction(TURN_END, false);
			nextAction();
		}
	}
	else
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	return true;
}

bool Player::pickupBallsGo()
{
	int y1, y2, y3;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	switch (currentDirY)
	{
	case Board::DownToUp:
		hand->SetAngle(3 * PI / 2);
		break;
	case Board::UpToDown:
		hand->SetAngle(PI / 2);
		break;
	default:
		insertAction(TURN_ABORT, true);
		return false;
	}

	hand->SetMagnitude(currentMagnitude);
	hand->Move();

	y1 = hand->getPosition()->y + hand->getDimension()->y / 2;
	y2 = dest->getPosition()->y + dest->getDimension()->y / 2;

	if (hand->GetDirection()->north)
	{
		if (y1 <= y2)
		{
			focus = dest;
			dest = focus->getOpposite();
			takeBalls(focus);
			y3 = y2 - y1;
			hand->getPosition()->y += y3;
			hand->SetMagnitude(0);
			currentDirY = Board::UpToDown;
			insertAction(PICKUP_BACK, true);
		}
	}
	else if (hand->GetDirection()->south)
	{
		if (y1 >= y2)
		{
			focus = dest;
			dest = focus->getOpposite();
			takeBalls(focus);
			y3 = y1 - y2;
			hand->getPosition()->y -= y3;
			hand->SetMagnitude(0);
			currentDirY = Board::DownToUp;
			insertAction(PICKUP_BACK, true);
		}
	}
	else
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	return true;
}

bool Player::pickupBallsStart()
{
	int ballsCount;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	if (!iterator.find(focus))
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	dest = focus->getOpposite();

	if (!dest)
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	ballsCount = dest->getBalls()->GetElementCount();

	if (!ballsCount)
	{
		nextAction();
		return true;
	}

	if (hand->getPosition()->y == dest->getPosition()->y)
	{
		insertAction(TURN_ABORT, true);
		return false;
	}

	if (hand->getPosition()->y < dest->getPosition()->y)
		currentDirY = Board::UpToDown;
	else
		currentDirY = Board::DownToUp;

	insertAction(PICKUP_GO, true);

	return true;
}

Player::Action Player::playTurn()
{
	if (!actionsIter)
		return NO_ACTION;

	switch (actionsIter->currentData())
	{
	case TURN_START:
		access = false;
		currentMagnitude = MIN_MAGNITUDE;
		if (displaceBallsStart())
			return TURN_START;
		else
			return NO_ACTION;
	case TURN_ABORT:
		access = true;
		abortTurn();
		return TURN_ABORT;
	case DISPLACE_MOVE:
		if (displaceBalls())
			return DISPLACE_MOVE;
		else
			return NO_ACTION;
	case DISPLACE_CONTINUE:
		if (displaceBallsContinue())
			return DISPLACE_CONTINUE;
		else
			return NO_ACTION;
	case PICKUP_START:
		if (pickupBallsStart())
			return PICKUP_START;
		else
			return NO_ACTION;
	case PICKUP_GO:
		if (pickupBallsGo())
			return PICKUP_GO;
		else
			return NO_ACTION;
	case PICKUP_BACK:
		if (pickupBallsBack())
			return PICKUP_BACK;
		else
			return NO_ACTION;
	case TURN_END:
		access = true;
		endTurn();
		return TURN_END;
	default:
		access = true;
		abortTurn();
		return NO_ACTION;
	}
}

void Player::prepareBoard()
{
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	hole = iterator.first();

	Sound::getSoundElement(BALLS_DROP, &sounds)->mute();

	while (hole)
	{
		focus = hole;
		ballsInHand.AddElement(new Ball(number));
		ballsInHand.AddElement(new Ball(number));
		releaseAllBalls();
		hole = iterator.next();
	}

	Sound::getSoundElement(BALLS_DROP, &sounds)->unmute();
}

void Player::prepareTestBoard(int *tab)
{
	int i, j;
	Hole *hole;
	LinkedList<Hole*>::Iterator iter(board->getHoles());

	i = 0;
	hole = iter.first();

	while (hole)
	{
		focus = hole;
		for (j = 0; j < tab[i]; j++)
			ballsInHand.AddElement(new Ball(number));
		releaseAllBalls();
		hole = iter.next();
		i++;
	}
}

bool Player::releaseAllBalls()
{
	return releaseBalls(ballsInHand.GetElementCount());
}

bool Player::releaseBalls(int n)
{
	int i;
	List balls;

	if (ballsInHand.GetElementCount() < n)
		return false;

	// Si pas de focus, remet les billes là où il les a prit
	if (!focus && actionsIter)
		switch (actionsIter->currentData())
		{
		case DISPLACE_MOVE:
			focus = src1;
			return releaseBalls(n);
		case PICKUP_BACK:
			focus = src2;
			return releaseBalls(n);
		default:
			return false;
		}

	for (i = 0; i < n; i++)
		balls.AddElement(new Ball(*Ball::getBallElement(i, &ballsInHand)));

	focus->addBalls(&balls);

	ballsInHand.RemoveElement(0, n, true);

	if (n > 1)
		Sound::getSoundElement(BALLS_DROP, &sounds)->play(DMUS_SEGF_SECONDARY);
	else
		Sound::getSoundElement(BALL_DROP, &sounds)->play(DMUS_SEGF_SECONDARY);

	return true;
}

void Player::resetVScore()
{
	Hole* hole;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	hole = iterator.first();

	while (hole)
	{
		hole->setVScore(-1);
		hole = iterator.next();
	}
}

void Player::startTurn(Board::Direction direction)
{
	currentDirX = direction;
	actionsIter = new LinkedList<int>::Iterator(&actions);

	insertAction(TURN_START, true);
}

bool Player::takeBalls(Hole *hole)
{
	int i;

	if (!focus | !actionsIter)
		return false;

	for (i = 0; i < hole->getBalls()->GetElementCount(); i++)
		ballsInHand.AddElement(new Ball(*Ball::getBallElement(i, hole->getBalls())));

	switch (actionsIter->currentData())
	{
	case TURN_START:
		src1 = hole;
		src1->removeBalls();
		break;
	case PICKUP_GO:
		src2 = hole;
		src2->removeBalls();
		break;
	case DISPLACE_CONTINUE:
		src1 = hole;
		src1->removeBalls();
		break;
	default:
		return false;
	}

	if (hole->getPlayerNb() == opponent->getNumber())
		Sound::getSoundElement(BALLS_GRAB, &sounds)->play(DMUS_SEGF_SECONDARY);

	return true;
}

int Player::tryFocus(int index, int holes)
{
	int height = 2;
	int width = holes / height;
	Node* node = new Node(width, height, focus);

	node->init(opponent->getBoard()->getHoles(), false);
	node->init(board->getHoles(), true);

	if (!node->simulate(index, true, 12))
		return -1;
	
	return node->getScore();
}

int Player::updateFocus()
{
	int i = 0;
	Hole *hole;
	LinkedList<Hole*>::Iterator iterator(board->getHoles());

	hole = iterator.first();

	while (hole)
	{
		if (hole->collision(hand))
		{
			focus = hole;
			return i;
		}
		hole = iterator.next();
		i++;
	}

	focus = NULL;

	return -1;
}
