#include "cnode.h"

Node::Node(Hole *hole, int w, int h)
{
	width = w;
	height = h;
	focus = hole;
	board = new int*[h];
	playerBoard = new int*[h];
	min = max = 0;

	for (int y = 0; y < h; y++)
	{
		board[y] = new int[w];
		playerBoard[y] = new int[w];
		for (int x = 0; x < w; x++)
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

void Node::init(LinkedList<Hole*> *holes, bool player)
{
	int x, y;
	LinkedList<Hole*>::Iterator iter(holes);
	Hole *hole = iter.first();

	x = y = 0;

	while (hole)
	{
		int balls = hole->getBalls()->GetElementCount();
		if (y == 0)
		{
			if (x < width - 1)
				x++;
			else
				y = 1;
		}
		else
			if (x > 0)
				x--;
		if (player)
		{
			playerBoard[y][x] = balls;
			min += balls;
		}
		else
		{
			board[y][x] = balls;
			max += balls;
		}
		hole = iter.next();
	}
}

void Node::simulate(int fromX, int fromY, bool player)
{
	int x = fromX;
	int y = fromY;
	int **myBoard = player ? playerBoard : board;
	int **opBoard = player ? board : playerBoard;
	int balls = myBoard[y][x];
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
		if (balls == 0)
			if (opBoard[0][x])
			{
				balls++;
				balls += opBoard[0][x];
				if (player)
					min += opBoard[0][x];
				else
					max += opBoard[0][x];
				opBoard[0][x] = 0;
			}
	}
}