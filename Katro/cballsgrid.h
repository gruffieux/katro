#ifndef CBALLSGRID
#define CBALLSGRID

#include <citem.h>
#include "cball.h"

class BallsGrid : public Item
{
public:
	class Cell : public Object
	{
	private:
		int x, y;
		bool locked;
		Ball *data;
	public:
		Cell()
		{
			x = y = 0;
			locked = false;
			data = NULL;
		}
		~Cell()
		{
		}
		static Cell* getCellElement(int index, List *pList) {return dynamic_cast<Cell*>(pList->GetElement(index));}
		Ball* getData() {return data;}
		int getX() {return x;}
		int getY() {return y;}
		bool isLocked() {return locked;}
		void lock() {locked = true;}
		void setData(Ball *data) {this->data = data;}
		void setX(int x) {this->x = x;}
		void setY(int y) {this->y = y;}
	};
	BallsGrid();
	BallsGrid(int totalX, int totalY, int cellsWidth, int cellsHeight, int posX, int posY);
	~BallsGrid();
	static BallsGrid* getBallsGridElement(int index, List *pList) {return dynamic_cast<BallsGrid*>(pList->GetElement(index));}
	bool contains(Ball *ball);
	bool isEmpty() {return ballsCount == 0;}
	bool isFull() {return ballsCount == max;}
	bool putAdjBall(Ball *ball);
	bool putCentralBall(Ball *ball);
private:
	int totalX, totalY, cellsWidth, cellsHeight, ballsCount, max;
	Cell **cells;
};

#endif
