#include "cnode.h"

Node::Node(int width, int height, Hole *focus)
{
	this->width = width;
	this->height = height;
	this->focus = focus;
	board = new int*[height];
	playerBoard = new int*[height];
	min = max = 0;

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

void Node::init(LinkedList<Hole*> *holes, bool player)
{
	int x = 0;
	int y = 1;
	LinkedList<Hole*>::Iterator iter(holes);
	Hole *hole = iter.first();

	// Le premier trou se trouve en bas à gauche et le dernier en haut à gauche
	while (hole)
	{
		int balls = hole->getBalls()->GetElementCount();
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

void Node::simulate(int index, bool player)
{
	int x, y;

	if (index < width)
	{
		y = 1;
		x = width - (width - index);
	}
	else
	{
		y = 0;
		x = width - index + width - 1;
	}
	
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
		if (balls == 0 && myBoard[y][x] > 1)
		{
			balls += myBoard[y][x];
			myBoard[y][x] = 0;
			if (y == 1 && opBoard[0][x])
			{
				balls += opBoard[0][x];
				if (player)
					min += opBoard[0][x];
				else
					max += opBoard[0][x];
				opBoard[0][x] = 0;
			}
		}
	}
}