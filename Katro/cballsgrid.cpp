#include <cinteger.h>
#include "cballsgrid.h"

BallsGrid::BallsGrid()
{
	totalX = totalY = cellsWidth = cellsHeight = ballsCount = max = 0;
	cells = NULL;
}

BallsGrid::BallsGrid(int totalX, int totalY, int cellsWidth, int cellsHeight, int posX, int posY) :
Item(posX, posY, totalX * cellsWidth, totalY * cellsHeight, Str())
{
	int x, y;

	this->totalX = totalX;
	this->totalY = totalY;
	this->cellsWidth = cellsWidth;
	this->cellsHeight = cellsHeight;
	ballsCount = max = 0;

	cells = new Cell*[totalX];

	for (x = 0; x < totalX; x++)
	{
		cells[x] = new Cell[totalY];
		for (y = 0; y < totalY; y++)
		{
			cells[x][y].setX(x * cellsWidth);
			cells[x][y].setY(y * cellsHeight);
			if ((x == 0 && y == 0) || (x == totalX-1 && y == 0) || (x == totalX-1 && y == totalY-1) || (x == 0 && y == totalY-1))
				cells[x][y].lock();
			else
				max++;
		}
	}
}

BallsGrid::~BallsGrid()
{
	int x;

	if (cells)
	{
		for (x = 0; x < totalX; x++)
			delete [] cells[x];
		delete [] cells;
	}
}

bool BallsGrid::contains(Ball *ball)
{
	int x, y;

	for (x = 0; x < totalX; x++)
		for (y = 0; y < totalY; y++)
			if (cells[x][y].getData() == ball)
				return true;

	return false;
}

bool BallsGrid::putAdjBall(Ball *ball)
{
	int i, x, y;
	Cell *cell;
	List busyCells, adjCells;

	// On cherche toutes les cellules occupées par une bille
	for (x = 0; x < totalX; x++)
		for (y = 0; y < totalY; y++)
			if (cells[x][y].getData())
				busyCells.AddElement(&cells[x][y]);

	// On cherche les 8 cellules adjacentes
	while (!adjCells.GetElementCount())
	{
		if (!busyCells.GetElementCount())
			return false;

		// On prend au hasard une cellule occupée
		i = Clock::random(0, busyCells.GetElementCount() - 1);
		cell = Cell::getCellElement(i, &busyCells);

		// On reprend l'index horizontal de la cellule
		if (cell->getX())
			x = cell->getX() / cellsWidth;
		else
			x = 0;

		// On reprend l'index vertical de la cellule
		if (cell->getY())
			y = cell->getY() / cellsHeight;
		else
			y = 0;

		// On ajoute la cellule adjacente en haut à gauche
		if (x && y)
			if (!cells[x-1][y-1].isLocked() && !cells[x-1][y-1].getData())
				adjCells.AddElement(&cells[x-1][y-1]);

		// On ajoute la cellule adjacente en haut
		if (y)
			if (!cells[x][y-1].isLocked() && !cells[x][y-1].getData())
				adjCells.AddElement(&cells[x][y-1]);

		// On ajoute la cellule adjacente en haut à droite
		if (x < totalX - 1 && y)
			if (!cells[x+1][y-1].isLocked() && !cells[x+1][y-1].getData())
				adjCells.AddElement(&cells[x+1][y-1]);

		// On ajoute la cellule adjacente à droite
		if (x < totalX - 1)
			if (!cells[x+1][y].isLocked() && !cells[x+1][y].getData())
				adjCells.AddElement(&cells[x+1][y]);

		// On ajoute la cellule adjacente en bas à droite
		if (x < totalX - 1 && y < totalY - 1)
			if (!cells[x+1][y+1].isLocked() && !cells[x+1][y+1].getData())
				adjCells.AddElement(&cells[x+1][y+1]);

		// On ajoute la cellule adjacente en bas
		if (y < totalY - 1)
			if (!cells[x][y+1].isLocked() && !cells[x][y+1].getData())
				adjCells.AddElement(&cells[x][y+1]);

		// On ajoute la cellule adjacente en bas à gauche
		if (x && y < totalY - 1)
			if (!cells[x-1][y+1].isLocked() && !cells[x-1][y+1].getData())
				adjCells.AddElement(&cells[x-1][y+1]);

		// On ajoute la cellule adjacente à gauche
		if (x)
			if (!cells[x-1][y].isLocked() && !cells[x-1][y].getData())
				adjCells.AddElement(&cells[x-1][y]);

		busyCells.RemoveElement(cell);
	}

	// On prend une cellule adjacente au hasard
	i = Clock::random(0, adjCells.GetElementCount() - 1);
	cell = Cell::getCellElement(i, &adjCells);

	// On positionne la bille sur la cellule
	ball->getPosition()->x = position.x + cell->getX();
	ball->getPosition()->y = position.y + cell->getY();

	// On définit la cellule comme occupée par la bille
	cell->setData(ball);

	ballsCount++;

	return true;
}

bool BallsGrid::putCentralBall(Ball *ball)
{
	int x, y, x1, y1, x2, y2;

	x = totalX / 2;
	y = totalY / 2;
	x1 = x - 1;
	y1 = y - 1;
	x2 = x;
	y2 = y;

	// Si le nombre de cellules horizontales est pair,
	// on place la bille dans une des 2 cellules centrales
	if (Integer(totalX).IsDivisable(2))
		x = Clock::random(x1, x2);

	// Si le nombre de cellules verticales est pair,
	// on place la bille dans une des 2 cellules centrales
	if (Integer(totalY).IsDivisable(2))
		y = Clock::random(y1, y2);

	if (cells[x][y].isLocked() || cells[x][y].getData())
		return false;

	// On positionne la bille sur la cellule
	ball->getPosition()->x = position.x + cells[x][y].getX();
	ball->getPosition()->y = position.y + cells[x][y].getY();

	// On définit la cellule comme occupée par la bille
	cells[x][y].setData(ball);

	ballsCount++;

	return true;
}
