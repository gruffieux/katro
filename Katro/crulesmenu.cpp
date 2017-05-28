#include <csceneslinker.h>
#include <canimation.h>
#include "crulesmenu.h"
#include "main.h"
#include "../KaRes/resource.h"

extern ScenesLinker scenario;

RulesMenu::RulesMenu() : Scene(RULES_MENU_ID)
{
	int i;

	title = new Visual();
	backButton = new Visual();
	text = new Visual();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i] = new Visual();
}

RulesMenu::~RulesMenu()
{
	int i;

	delete title;
	delete backButton;
	delete text;

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		delete bottomDeco[i];
}

void RulesMenu::destroyAni()
{
	int i;

	title->destroyAni();
	backButton->destroyAni();
	text->destroyAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->destroyAni();
}

void RulesMenu::doMouseEvents(Cursor *cursor)
{
	if (cursor->buttonReleased() == Cursor::LEFT_CLICK)
		if (cursor->collision(backButton))
			scenario.goToScene(MAIN_MENU_ID);
}

void RulesMenu::init()
{
	int i, x;
	double d;
	bool b;

	title->getDimension()->x = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->x;
	title->getDimension()->y = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->y;
	title->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), 250);

	backButton->getDimension()->x = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->x;
	backButton->getDimension()->y = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->y;
	backButton->rescale(70, 70);
	backButton->getPosition()->x = 10;
	backButton->getPosition()->y = 10;

	text->getDimension()->x = Animation::getAnimationElement(0, text->getAnimations())->GetFrameDimension()->x;
	text->getDimension()->y = Animation::getAnimationElement(0, text->getAnimations())->GetFrameDimension()->y;
	d = (double)text->getDimension()->x / (double)displayer->GetCurrentMode()->GetWidth() * 100;
	if (d > 100)
	{
		x = 100 - ((int)d - 100);
		text->rescale(x, x);
	}
	text->centerPosition(Axe(displayer->GetCurrentMode()->GetWidth(), displayer->GetCurrentMode()->GetHeight()));
	text->getPosition()->y = title->getPosition()->y + title->getDimension()->y;

	x = displayer->GetCurrentMode()->GetWidth() / 2 - (80 * (TOTAL_BOTTOM_DECO * 2 - 1) / 2);
	b = true;

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
	{
		bottomDeco[i]->getDimension()->x = Animation::getAnimationElement(0, bottomDeco[i]->getAnimations())->GetFrameDimension()->x;
		bottomDeco[i]->getDimension()->y = Animation::getAnimationElement(0, bottomDeco[i]->getAnimations())->GetFrameDimension()->y;
		if (b)
			bottomDeco[i]->rescale(15, 15);
		else
			bottomDeco[i]->rescale(30, 30);
		bottomDeco[i]->getPosition()->x = x;
		bottomDeco[i]->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - 70;
		x += 80 * 2;
		b = !b;
	}
}

void RulesMenu::initAni()
{
	int i;
	bool b = true;

	title->addAni(IDB_TITLE2, "", 1, 641, 220, -1, 0);
	backButton->addAni(IDB_BACK_BUTTON, "", 2, 368, 73, RGB(255, 255, 255), 0);
	text->addAni(IDB_RULES, "", 1, 1290, 692, 0, 0);

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
	{
		if (b)
			bottomDeco[i]->addAni(IDB_STONES, "", 1, 736, 393, RGB(255, 255, 255), 0);
		else
			bottomDeco[i]->addAni(IDB_BUSH, "", 1, 360, 196, RGB(255, 255, 255), 0);
		b = !b;
	}
}

void RulesMenu::play()
{
}

void RulesMenu::restoreAni()
{
	int i;

	title->restoreAni();
	backButton->restoreAni();
	text->restoreAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->restoreAni();
}

void RulesMenu::runAnimations()
{
	int i;

	title->runAnimations();
	backButton->runAnimations();
	text->runAnimations();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->runAnimations();
}
