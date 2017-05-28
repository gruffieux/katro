#include <csceneslinker.h>
#include <canimation.h>
#include "cgamemenu.h"
#include "cgamescene.h"
#include "main.h"
#include "../KaRes/resource.h"

extern ScenesLinker scenario;

GameMenu::GameMenu() : Scene(GAME_MENU_ID)
{
	int i;

	title = new Visual();
	palm = new Visual();
	coconuts = new Visual();
	backButton = new Visual();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i] = new Visual();

	buttons.AddElement(new Visual());
	buttons.AddElement(new Visual());
	buttons.AddElement(new Visual());
}

GameMenu::~GameMenu()
{
	int i;

	delete title;
	delete palm;
	delete coconuts;
	delete backButton;

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		delete bottomDeco[i];

	buttons.RemoveAllElement(0, true);
}

void GameMenu::destroy()
{
}

void GameMenu::destroyAni()
{
	int i;

	title->destroyAni();
	palm->destroyAni();
	coconuts->destroyAni();
	backButton->destroyAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->destroyAni();

	for (i = 0; i < buttons.GetElementCount(); i++)
		getVisualElement(i, &buttons)->destroyAni();
}

void GameMenu::doMouseEvents(Cursor *cursor)
{
	int i;
	Visual *button;

	if (cursor->buttonReleased() == Cursor::LEFT_CLICK)
	{
		if (cursor->collision(backButton))
			scenario.goToPreviousScene();

		for (i = 0; i < buttons.GetElementCount(); i++)
		{
			button = Visual::getVisualElement(i, &buttons);
			if (button->getVisible() && cursor->collision(button))
				switch (i)
				{
				case 0:
					// On clique sur le bouton "charger partie"
					GameScene::load(cursor->getMover());
					break;
				case 1:
					// On clique sur le bouton "continuer partie"
					scenario.goToScene(GAME_SCENE_ID);
					break;
				case 2:
					// On clique sur le bouton "nouvelle partie"
					scenario.goToScene(PLAYER_MENU_ID);
					break;
				}
		}
	}
}

void GameMenu::init()
{
	int i, x;
	bool b;
	Visual *button;

	title->getDimension()->x = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->x;
	title->getDimension()->y = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->y;
	title->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), 250);

	palm->getDimension()->x = Animation::getAnimationElement(0, palm->getAnimations())->GetFrameDimension()->x;
	palm->getDimension()->y = Animation::getAnimationElement(0, palm->getAnimations())->GetFrameDimension()->y;
	palm->rescale(75, 75);
	palm->getPosition()->x = 0;
	palm->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - palm->getDimension()->y;

	backButton->getDimension()->x = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->x;
	backButton->getDimension()->y = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->y;
	backButton->rescale(70, 70);
	backButton->getPosition()->x = 10;
	backButton->getPosition()->y = 10;

	for (i = 0; i < buttons.GetElementCount(); i++)
	{
		button = getVisualElement(i, &buttons);
		button->getDimension()->x = Animation::getAnimationElement(0, button->getAnimations())->GetFrameDimension()->x;
		button->getDimension()->y = Animation::getAnimationElement(0, button->getAnimations())->GetFrameDimension()->y;
		button->rescale(70, 70);
		button->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), displayer->GetCurrentMode()->GetHeight());
		if (i)
			button->getPosition()->y = getItemElement(i-1, &buttons)->getPosition()->y - button->getDimension()->y - 10;
		else
			button->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - button->getDimension()->y - 150;
		switch (i)
		{
		case 0:
			if (GameScene::saveExists())
				button->show();
			else
				button->hide();
			break;
		case 1:
			if (scenario.findScene(GAME_SCENE_ID))
				button->show();
			else
				button->hide();
			break;
		case 2:
			button->show();
			break;
		}
	}

	coconuts->getDimension()->x = Animation::getAnimationElement(0, coconuts->getAnimations())->GetFrameDimension()->x;
	coconuts->getDimension()->y = Animation::getAnimationElement(0, coconuts->getAnimations())->GetFrameDimension()->y;
	coconuts->rescale(40, 40);
	coconuts->getPosition()->x = displayer->GetCurrentMode()->GetWidth() - coconuts->getDimension()->x - 50;
	coconuts->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - coconuts->getDimension()->y - 150;

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

void GameMenu::initAni()
{
	int i;
	bool b = true;

	title->addAni(IDB_TITLE2, "", 1, 641, 220, -1, 0);
	palm->addAni(IDB_PALM, "", 1, 573, 900, RGB(255, 255, 255), 0);
	coconuts->addAni(IDB_COCONUTS, "", 1, 500, 559, RGB(255, 255, 255), 0);
	backButton->addAni(IDB_BACK_BUTTON, "", 2, 368, 73, RGB(255, 255, 255), 0);

	for (i = 0; i < buttons.GetElementCount(); i++)
	{
		getVisualElement(i, &buttons)->addAni(IDB_GAMEMENU_BUTTONS, "", 3, 453, 98, RGB(255, 255, 255), 0);
		getVisualElement(i, &buttons)->addAni(IDB_GAMEMENU_BUTTONS_GRAYED, "", 3, 453, 98, RGB(255, 255, 255), 0);
	}

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
	{
		if (b)
			bottomDeco[i]->addAni(IDB_STONES, "", 1, 736, 393, RGB(255, 255, 255), 0);
		else
			bottomDeco[i]->addAni(IDB_BUSH, "", 1, 360, 196, RGB(255, 255, 255), 0);
		b = !b;
	}
}

void GameMenu::play()
{
}

void GameMenu::restoreAni()
{
	int i;

	title->restoreAni();
	palm->restoreAni();
	coconuts->restoreAni();
	backButton->restoreAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->restoreAni();

	for (i = 0; i < buttons.GetElementCount(); i++)
		getVisualElement(i, &buttons)->restoreAni();
}

void GameMenu::runAnimations()
{
	int i;
	Visual *button;

	title->runAnimations();
	coconuts->runAnimations();
	backButton->runAnimations();
	palm->runAnimations();

	for (i = 0; i < buttons.GetElementCount(); i++)
	{
		button = Visual::getVisualElement(i, &buttons);
		button->setVisibleZone();
		if (button->getVisible())
			button->runAni(0, i);
		else
			button->runAni(1, i);
	}

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->runAnimations();
}
