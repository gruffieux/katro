#include "cplayeria.h"
#include "cball.h"

PlayerIA::PlayerIA(Str name) : Player(2, name)
{
	level = 2;
}

PlayerIA::~PlayerIA()
{
}

int PlayerIA::minimax(Node *node, int holes, int depth, bool maximizingPlayer, int alpha, int beta)
{
	int value = 0;
	int height = 2;
	int width = holes / height;

	LinkedList<Hole*>::Iterator iter(board->getHoles());
	Hole* hole = iter.first();
	int i = 0;
	while (hole)
	{
		if (hole->getBalls()->GetElementCount())
		{
			Node* child = new Node(width, height, hole);
			child->init(board->getHoles(), false);
			child->init(opponent->getBoard()->getHoles(), true);
			if (!child->simulate(i, !maximizingPlayer, level * 4))
			{
				hole = iter.next();
				i++;
				continue;
			}
			node->getChilds()->AddElement(child);
		}
		hole = iter.next();
		i++;
	}

	if (depth == 0 || node->hasChild() == false)
		return node->getScore();

	if (maximizingPlayer)
	{
		value = -1;
		for (int i = 0; i < node->getChilds()->GetElementCount(); i++)
		{
			Node *child = Node::getNodeElement(node->getChilds(), i);
			value = max(value, minimax(child, holes, depth - 1, false, alpha, beta));
			if (value >= beta)
			{
				node->getChilds()->RemoveAllElement(i + 1, true);
				return value;
			}
			alpha = max(alpha, value);
		}
	}
	else
	{
		value = INFINITY;
		for (int i = 0; i < node->getChilds()->GetElementCount(); i++)
		{
			Node* child = Node::getNodeElement(node->getChilds(), i);
			value = min(value, minimax(child, holes, depth - 1, true, alpha, beta));
			if (value <= alpha)
			{
				node->getChilds()->RemoveAllElement(i + 1, true);
				return value;
			}
			beta = min(beta, value);
		}
	}

	return value;
}

void PlayerIA::think(int holes)
{
	int i = 0;
	Node* node = new Node();
	int value = minimax(node, holes, level-1, true, -1, INFINITY);

	focus = NULL;

	if (node->getChilds()->GetElementCount())
	{
		int max = holes;
		if (level > 1)
			max /= level == 3 ? holes : 2;
		node->getChilds()->filterHighestScore(max);
		i = Clock::random(0, node->getChilds()->GetElementCount() - 1);
		focus = Node::getNodeElement(node->getChilds(), i)->getFocus();
	}
	else
	{
		Hole* hole;
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
			i = Clock::random(0, holes.GetElementCount() - 1);
			focus = holes.getHoleElement(i);
		}
	}
}
