#include <csceneslinker.h>
#include <canimation.h>
#include <csound.h>
#include "cmainmenu.h"
#include "cgamemenu.h"
#include "main.h"
#include "../KaRes/resource.h"

extern ScenesLinker scenario;
extern List musics;

MainMenu::MainMenu() : Scene(MAIN_MENU_ID)
{
	int i;

	title = new Visual();
	palm = new Visual();
	board = new Visual();
	coconuts = new Visual();
	playButton = new Visual();
	rulesButton = new Visual();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i] = new Visual();
}

MainMenu::~MainMenu()
{
	int i;

	delete title;
	delete palm;
	delete board;
	delete coconuts;
	delete playButton;
	delete rulesButton;

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		delete bottomDeco[i];
}

void MainMenu::destroy()
{
}

void MainMenu::destroyAni()
{
	int i;

	title->destroyAni();
	palm->destroyAni();
	board->destroyAni();
	coconuts->destroyAni();
	playButton->destroyAni();
	rulesButton->destroyAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->destroyAni();
}

void MainMenu::doMouseEvents(Cursor *cursor)
{
	GameMenu *gameMenu;

	if (cursor->buttonReleased() == Cursor::LEFT_CLICK)
	{
		if (cursor->collision(playButton))
		{
			gameMenu = (GameMenu*)scenario.findScene(GAME_MENU_ID);
			if (gameMenu)
			{
				gameMenu->destroy();
				gameMenu->destroyAni();
				scenario.removeScene(gameMenu);
				delete gameMenu;
			}
			gameMenu = new GameMenu();
			gameMenu->initAni();
			scenario.insertScene(gameMenu);
			scenario.goToNextScene();
		}

		if (cursor->collision(rulesButton))
			scenario.goToScene(RULES_MENU_ID);
	}
}

void MainMenu::init()
{
	int i, x;
	bool b;

	title->getDimension()->x = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->x;
	title->getDimension()->y = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->y;
	title->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), 250);

	palm->getDimension()->x = Animation::getAnimationElement(0, palm->getAnimations())->GetFrameDimension()->x;
	palm->getDimension()->y = Animation::getAnimationElement(0, palm->getAnimations())->GetFrameDimension()->y;
	palm->rescale(75, 75);
	palm->getPosition()->x = 0;
	palm->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - palm->getDimension()->y;

	board->getDimension()->x = Animation::getAnimationElement(0, board->getAnimations())->GetFrameDimension()->x;
	board->getDimension()->y = Animation::getAnimationElement(0, board->getAnimations())->GetFrameDimension()->y;
	board->rescale(60, 60);
	board->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), displayer->GetCurrentMode()->GetHeight());
	board->getPosition()->x -= 150;
	board->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - board->getDimension()->y - 150;

	rulesButton->getDimension()->x = Animation::getAnimationElement(0, rulesButton->getAnimations())->GetFrameDimension()->x;
	rulesButton->getDimension()->y = Animation::getAnimationElement(0, rulesButton->getAnimations())->GetFrameDimension()->y;
	rulesButton->rescale(70, 70);
	rulesButton->getPosition()->x = displayer->GetCurrentMode()->GetWidth() - rulesButton->getDimension()->x - 100;
	rulesButton->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - rulesButton->getDimension()->y - 150;

	playButton->getDimension()->x = Animation::getAnimationElement(0, playButton->getAnimations())->GetFrameDimension()->x;
	playButton->getDimension()->y = Animation::getAnimationElement(0, playButton->getAnimations())->GetFrameDimension()->y;
	playButton->rescale(70, 70);
	playButton->getPosition()->x = displayer->GetCurrentMode()->GetWidth() - playButton->getDimension()->x - 100;
	playButton->getPosition()->y = rulesButton->getPosition()->y - playButton->getDimension()->y - 10;

	coconuts->getDimension()->x = Animation::getAnimationElement(0, coconuts->getAnimations())->GetFrameDimension()->x;
	coconuts->getDimension()->y = Animation::getAnimationElement(0, coconuts->getAnimations())->GetFrameDimension()->y;
	coconuts->rescale(20, 20);
	coconuts->centerPosition(playButton);
	coconuts->getPosition()->y = playButton->getPosition()->y - 10 - coconuts->getDimension()->y;

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

	if (!Sound::getSoundElement(MANDRY_A, &musics)->isPlaying())
		Sound::getSoundElement(MANDRY_A, &musics)->play(DMUS_SEGF_DEFAULT, false);
}

void MainMenu::initAni()
{
	int i;
	bool b = true;

	title->addAni(IDB_TITLE2, "", 1, 641, 220, -1, 0);
	palm->addAni(IDB_PALM, "", 1, 573, 900, RGB(255, 255, 255), 0);
	board->addAni(IDB_MAINMENU_BOARD2P, "", 1, 480, 480, -1, 0);
	coconuts->addAni(IDB_COCONUTS, "", 1, 500, 559, RGB(255, 255, 255), 0);
	playButton->addAni(IDB_MAINMENU_BUTTONS, "", 2, 441, 181, RGB(255, 255, 255), 0);
	rulesButton->addAni(IDB_MAINMENU_BUTTONS, "", 2, 441, 181, RGB(255, 255, 255), 0);

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
	{
		if (b)
			bottomDeco[i]->addAni(IDB_STONES, "", 1, 736, 393, RGB(255, 255, 255), 0);
		else
			bottomDeco[i]->addAni(IDB_BUSH, "", 1, 360, 196, RGB(255, 255, 255), 0);
		b = !b;
	}
}

void MainMenu::play()
{
}

void MainMenu::restoreAni()
{
	int i;

	title->restoreAni();
	palm->restoreAni();
	board->restoreAni();
	coconuts->restoreAni();
	playButton->restoreAni();
	rulesButton->restoreAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->restoreAni();
}

void MainMenu::runAnimations()
{
	int i;

	title->runAnimations();
	board->runAnimations();

	coconuts->runAnimations();
	palm->runAnimations();
	playButton->runAnimations();

	rulesButton->setVisibleZone();
	rulesButton->runAni(0, 1);

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->runAnimations();
}
