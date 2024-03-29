#include "cnodelist.h"

Node::Node()
{
	width = height = score = 0;
	focus = NULL;
	board = playerBoard = NULL;
}

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

	// Le premier trou se trouve en bas � gauche et le dernier en haut � gauche
	while (hole)
	{
		int balls = hole->getBalls()->GetElementCount();
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

void Node::init(int **board, int **playerBoard)
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < 2; y++)
		{
			this->board[y][x] = board[y][x];
			this->playerBoard[y][x] = playerBoard[y][x];
		}
}

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

	int y1 = player ? 0 : 1;
	int y2 = player ? 1 : 0;
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
			if (y == y1 && opBoard[y2][x])
			{
				balls += opBoard[y2][x];
				opBoard[y2][x] = 0;
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

	score = 0;
	for (x = 0; x < width; x++)
		for (y = 0; y < 2; y++)
			score += board[y][x];

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

bool NodeList::filterHighestScore(int index)
{
	int i = 0;

	while (i < ElementCount)
		if (!Node::getNodeElement(this, i)->getFocus()->getBalls()->GetElementCount())
			RemoveElement(i, 1, true);
		else
			i++;

	OrderBy(NodeList::ORDER_BY_SCORE);
	ReverseOrder();

	return RemoveAllElement(index, true);
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
