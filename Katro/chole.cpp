#include "chole.h"
#include "cball.h"
#include "cballsgrid.h"
#include "../KaRes/resource.h"

Hole::Hole()
{
	playerNb = 0;
	vScore = -1;
	rank = FRONT;
	next = previous = opposite = 0;
}

Hole::Hole(int playerNb) : Item(0, 0, 128, 128, Str())
{
	this->playerNb = playerNb;
	vScore = -1;
	rank = FRONT;
	next = previous = opposite = 0;
}

Hole::Hole(Hole &model)
{
	int i;

	playerNb = model.getPlayerNb();
	vScore = model.getVScore();
	rank = model.getRank();
	next = model.getNext();
	previous = model.getPrevious();
	opposite = model.getOpposite();

	for (i = 0; i < model.getBalls()->GetElementCount(); i++)
		balls.AddElement(new Ball(*Ball::getBallElement(i, model.getBalls())));
}

Hole::~Hole()
{
}

bool Hole::addBalls(List *pList)
{
	int i;
	Ball *ball;

	for (i = 0; i < pList->GetElementCount(); i++)
	{
		ball = new Ball(*Ball::getBallElement(i, pList));
		if (!balls.AddElement(ball))
			return false;
		ball->initAni();
	}

	layBalls();
	updateIndicator();

	return true;
}

void Hole::clear()
{
	int i;

	for (i = 0; i < balls.GetElementCount(); i++)
		Ball::getBallElement(i, &balls)->destroyAni();

	balls.RemoveAllElement(0, true);
}

void Hole::destroyAni()
{
	int i;

	Visual::destroyAni();

	indicator.Destroy();

	for (i = 0; i < balls.GetElementCount(); i++)
		Ball::getBallElement(i, &balls)->destroyAni();
}

void Hole::initAni()
{
	addAni(IDB_HOLES, "", 2, dimension.x, dimension.y, RGB(255, 255, 255), 90);
}

void Hole::layBalls()
{
	int i, j, n;
	Ball *ball;
	BallsGrid *layer;

	i = j = 0;
	n = 3;

	// On dispose chaque bille à l'intérieur d'une grille et par couche
	// Plus la couche est élevée, plus la grille est grande (+ une cellule au carré)
	// La première bille de la couche est placée dans une cellule centrale de la grille
	// Les billes suivantes vont dans une des 8 cellules libres et adjacentes à une cellule occupée
	while (i < balls.GetElementCount())
	{
		ball = Ball::getBallElement(i, &balls);

		if (!layers.GetElementCount())
		{
			layer = new BallsGrid(n, n, ball->getDimension()->x, ball->getDimension()->y, 0, 0);
			layer->centerPosition(this);
			layers.AddElement(layer);
		}
		else
			layer = BallsGrid::getBallsGridElement(j, &layers);

		if (layer->contains(ball))
			i++;
		else
			if (layer->isEmpty())
			{
				layer->putCentralBall(ball);
				i++;
			}
			else
			{
				if (layer->isFull())
				{
					j++;
					n++;
					if (j >= layers.GetElementCount())
					{
						layer = new BallsGrid(n, n, ball->getDimension()->x, ball->getDimension()->y, 0, 0);
						layer->centerPosition(this);
						layers.AddElement(layer);
					}
					else
						layer = BallsGrid::getBallsGridElement(j, &layers);
				}
				else
				{
					layer->putAdjBall(ball);
					i++;
				}
			}
	}
}

void Hole::link(LinkedList<Hole*> *holes)
{
	LinkedList<Hole*>::Iterator iterator(holes);

	previous = iterator.last();

	if (previous)
		previous->setNext(this);

	holes->append(this);
}

void Hole::removeBalls()
{
	int i;

	for (i = 0; i < balls.GetElementCount(); i++)
		Ball::getBallElement(i, &balls)->destroyAni();

	layers.RemoveAllElement(0, true);
	balls.RemoveAllElement(0, true);

	layBalls();
	updateIndicator();
}

void Hole::runAnimations()
{
	int i;

	setVisibleZone();
	runAni(0, playerNb - 1);

	for (i = 0; i < balls.GetElementCount(); i++)
		Ball::getBallElement(i, &balls)->runAnimations();

	// Affiche l'indicateur de billes
	displayer->Blit(position.x, position.y, &indicator, NULL, NULL, DDBLT_WAIT | DDBLT_KEYSRC);
}

void Hole::restoreAni()
{
	int i;

	Visual::restoreAni();

	for (i = 0; i < balls.GetElementCount(); i++)
		Ball::getBallElement(i, &balls)->restoreAni();

	updateIndicator();
}

void Hole::updateIndicator()
{
	displayer->LoadText(&indicator, NULL, Str::Parse(balls.GetElementCount()), RGB(0, 0, 0), RGB(255, 255, 255));
	indicator.SetColorKey(RGB(0, 0, 0));
}
