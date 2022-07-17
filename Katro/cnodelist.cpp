#include "cnodelist.h"

Node::Node(int width, int height, Hole* focus)
{
	this->width = width;
	this->height = height;
	this->focus = focus;
	board = new int* [height];
	playerBoard = new int* [height];
	score = 0;

	for (int y = 0; y < height; y++)
	{
		board[y] = new int[width];
		playerBoard[y] = new int[width];
		for (int x = 0; x < width; x++)
		{
			board[y][x] = 0;
			playerBoard[y][x] = 0;
		}
	}
}

Node::~Node()
{
	for (int y = 0; y < height; y++)
	{
		delete board[y];
		delete playerBoard[y];
	}

	delete board;
	delete playerBoard;
}

void Node::init(LinkedList<Hole*>* holes, bool player)
{
	int x = 0;
	int y = 1;
	LinkedList<Hole*>::Iterator iter(holes);
	Hole* hole = iter.first();
	score = 0;

	// Le premier trou se trouve en bas à gauche et le dernier en haut à gauche
	while (hole)
	{
		int balls = hole->getBalls()->GetElementCount();
		score += balls;
		if (player)
			playerBoard[y][x] = balls;
		else
			board[y][x] = balls;
		if (y == 1)
		{
			if (x < width - 1)
				x++;
			else
				y = 0;
		}
		else
			if (x > 0)
				x--;
		hole = iter.next();
	}
}

// TODO: Score bonus pour les billes à l'arrière et malus pour les billes à l'avant
bool Node::simulate(int index, bool player, int maxRounds)
{
	int x, y, xStart, yStart;

	if (index < width)
	{
		y = yStart = 1;
		x = xStart = width - (width - index);
	}
	else
	{
		y = yStart = 0;
		x = xStart = width - index + width - 1;
	}

	int** myBoard = player ? playerBoard : board;
	int** opBoard = player ? board : playerBoard;
	int balls = myBoard[y][x];
	int rounds = 0;
	myBoard[y][x] = 0;

	while (balls)
	{
		if (y == 0)
		{
			if (x < width - 1)
				x++;
			else
				y++;
		}
		else
			if (x > 0)
				x--;
			else
				y--;
		myBoard[y][x]++;
		balls--;
		if (balls == 0 && myBoard[y][x] > 1)
		{
			balls += myBoard[y][x];
			myBoard[y][x] = 0;
			if (y == 1 && opBoard[0][x])
			{
				balls += opBoard[0][x];
				score += balls;
				opBoard[0][x] = 0;
			}
		}
		if (y == yStart && x == xStart)
			rounds++;
		if (rounds >= maxRounds)
		{
			score = 0;
			return false;
		}	
	}

	return true;
}

NodeList::NodeList()
{
}

NodeList::NodeList(int order) : List(order)
{
}

NodeList::NodeList(NodeList& model) : List(model)
{
}

NodeList::~NodeList()
{
}

int NodeList::OrderBy(int NewOrder, Container* pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_SCORE:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = Node::getNodeElement(this, i)->getScore();
			result = OrderElements(ORDER_NUM);
			if (!result)
				return result;
			break;
		default:
			return ORDER_UNKNOWN;
		}
	}
	else
		return result;

	FinalizeOrder(NewOrder);

	return result;
}
