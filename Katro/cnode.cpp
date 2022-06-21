#include "cnode.h"

Node::Node(int w, int h, int b)
{
	width = w;
	height = h;
	ballsPerHole = b;
	board = new int*[h];
	playerBoard = new int*[h];

	for (int y = 0; y < h; y++)
	{
		board[y] = new int[w];
		playerBoard[y] = new int[w];
		for (int x = 0; x < w; x++)
		{
			board[y][x] = b;
			playerBoard[y][x] = b;
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

int Node::simulate(int fromX, int fromY)
{
	int balls = board[fromY][fromX];

	while (balls)
	{
	}

	return 0;
}