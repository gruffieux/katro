#include <csceneslinker.h>
#include <canimation.h>
#include "cgamescene.h"
#include "cplayermenu.h"
#include "main.h"
#include "../KaRes/resource.h"

extern ScenesLinker scenario;
extern Cursor *cursor;

PlayerMenu::PlayerMenu() : Scene(PLAYER_MENU_ID)
{
	int i;

	title = new Visual();
	palm = new Visual();
	coconuts = new Visual();
	backButton = new Visual();
	holeText = new Visual();
	playerDlg = NULL;

	for (i = 0; i < TOTAL_BUTTON; i++)
		button[i] = new Visual();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i] = new Visual();

	for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
	{
		holesChoice[i] = new RadioButton();
		holeNb[i] = new Visual();
	}
}

PlayerMenu::~PlayerMenu()
{
	int i;

	delete title;
	delete palm;
	delete coconuts;
	delete backButton;
	delete holeText;

	if (playerDlg)
		delete playerDlg;

	for (i = 0; i < TOTAL_BUTTON; i++)
		delete button[i];

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		delete bottomDeco[i];

	for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
	{
		delete holesChoice[i];
		delete holeNb[i];
	}
}

void PlayerMenu::destroy()
{
}

void PlayerMenu::destroyAni()
{
	int i;

	title->destroyAni();
	palm->destroyAni();
	coconuts->destroyAni();
	backButton->destroyAni();
	holeText->destroyAni();

	if (playerDlg)
	{
		playerDlg->DestroyText();
		playerDlg->destroyAni();
	}

	for (i = 0; i < TOTAL_BUTTON; i++)
		button[i]->destroyAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->destroyAni();

	for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
	{
		holesChoice[i]->destroyAni();
		holeNb[i]->destroyAni();
	}
}

void PlayerMenu::destroyPlayerDlg()
{
	playerDlg->DestroyText();
	playerDlg->destroyAni();

	delete playerDlg;

	playerDlg = NULL;
}

void PlayerMenu::doMouseEvents(Cursor *cursor)
{
	int i, j, holesTotal;
	GameScene *gameScene;

	if (cursor->buttonReleased() == Cursor::LEFT_CLICK)
	{
		if (cursor->collision(backButton))
			scenario.goToScene(GAME_MENU_ID);

		for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
			if (cursor->collision(holesChoice[i]))
			{
				holesChoice[i]->focus();
				for (j = 0; j < TOTAL_HOLES_CHOICE; j++)
					if (j != i)
						holesChoice[j]->unfocus();
				break;
			}

		for (i = 0; i < TOTAL_BUTTON; i++)
			if (cursor->collision(button[i]))
			{
				if (i == GameScene::ONE_PLAYER_VS_ONE_CPU)
					initPlayerDlg(8, "", "Ordinateur");
				else
					initPlayerDlg(5);
				gameScene = (GameScene*)scenario.findScene(GAME_SCENE_ID);
				if (gameScene)
				{
					gameScene->destroy();
					gameScene->destroyAni();
					scenario.removeScene(gameScene);
					scenario.goToScene(number);
					delete gameScene;
				}
				if (holesChoice[0]->getChecked())
					holesTotal = 8;
				else if (holesChoice[1]->getChecked())
					holesTotal = 12;
				else
					holesTotal = 8;
				switch (i)
				{
				case GameScene::ONE_PLAYER_VS_ONE_CPU:
					gameScene = new GameScene(holesTotal, GameScene::ONE_PLAYER_VS_ONE_CPU, GameScene::TWO_PLAYERS, cursor->getMover());
					break;
				case GameScene::ONE_PLAYER_VS_ONE_PLAYER:
					gameScene = new GameScene(holesTotal, GameScene::ONE_PLAYER_VS_ONE_PLAYER, GameScene::TWO_PLAYERS, cursor->getMover());
					break;
				default:
					gameScene = NULL;
					break;
				}
				if (gameScene)
				{
					gameScene->initAni();
					scenario.insertScene(gameScene);
				}
				break;
			}
	}
}

void PlayerMenu::init()
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

	backButton->getDimension()->x = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->x;
	backButton->getDimension()->y = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->y;
	backButton->rescale(70, 70);
	backButton->getPosition()->x = 10;
	backButton->getPosition()->y = 10;

	for (i = 0; i < TOTAL_BUTTON; i++)
	{
		button[i]->getDimension()->x = Animation::getAnimationElement(0, button[i]->getAnimations())->GetFrameDimension()->x;
		button[i]->getDimension()->y = Animation::getAnimationElement(0, button[i]->getAnimations())->GetFrameDimension()->y;
		button[i]->rescale(70, 70);
		button[i]->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), displayer->GetCurrentMode()->GetHeight());
		if (i)
			button[i]->getPosition()->y = button[i-1]->getPosition()->y - button[i]->getDimension()->y - 10;
		else
			button[i]->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - button[i]->getDimension()->y - 150;
	}

	b = false;

	for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
	{
		holeNb[i]->getDimension()->x = Animation::getAnimationElement(0, holeNb[i]->getAnimations())->GetFrameDimension()->x;
		holeNb[i]->getDimension()->y = Animation::getAnimationElement(0, holeNb[i]->getAnimations())->GetFrameDimension()->y;
		holeNb[i]->rescale(60, 60);
		holesChoice[i]->getDimension()->x = Animation::getAnimationElement(0, holesChoice[i]->getAnimations())->GetFrameDimension()->x;
		holesChoice[i]->getDimension()->y = Animation::getAnimationElement(0, holesChoice[i]->getAnimations())->GetFrameDimension()->y;
		holeNb[i]->getPosition()->y = button[TOTAL_BUTTON-1]->getPosition()->y - holeNb[i]->getDimension()->y - 10;
		holesChoice[i]->getPosition()->y = button[TOTAL_BUTTON-1]->getPosition()->y - holesChoice[i]->getDimension()->y - 10;
		if (i)
			holesChoice[i]->getPosition()->x = holesChoice[i-1]->getPosition()->x + holesChoice[i]->getDimension()->x + 20;
		else
			holesChoice[i]->getPosition()->x = button[TOTAL_BUTTON-1]->getPosition()->x;
		holeNb[i]->centerPosition(holesChoice[i]);
		if (!b && holesChoice[i]->getChecked())
			b = true;
	}

	if (!b)
		holesChoice[0]->focus();

	holeText->getDimension()->x = Animation::getAnimationElement(0, holeText->getAnimations())->GetFrameDimension()->x;
	holeText->getDimension()->y = Animation::getAnimationElement(0, holeText->getAnimations())->GetFrameDimension()->y;
	holeText->getPosition()->x = holesChoice[TOTAL_HOLES_CHOICE-1]->getPosition()->x + holesChoice[TOTAL_HOLES_CHOICE-1]->getDimension()->x + 20;
	holeText->getPosition()->y = button[TOTAL_BUTTON-1]->getPosition()->y - holeText->getDimension()->y - 10;

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

void PlayerMenu::initAni()
{
	int i;
	bool b = true;

	title->addAni(IDB_TITLE2, "", 1, 641, 220, -1, 0);
	palm->addAni(IDB_PALM, "", 1, 573, 900, RGB(255, 255, 255), 0);
	coconuts->addAni(IDB_COCONUTS, "", 1, 500, 559, RGB(255, 255, 255), 0);
	backButton->addAni(IDB_BACK_BUTTON, "", 2, 368, 73, RGB(255, 255, 255), 0);
	holeText->addAni(IDB_HOLE_TEXT, "", 1, 171, 38, RGB(0, 0, 0), 0);
	button[GameScene::ONE_PLAYER_VS_ONE_CPU]->addAni(IDB_PLAYERMENU_BUTTON1, "", 1, 783, 98, RGB(255, 255, 255), 0);
	button[GameScene::ONE_PLAYER_VS_ONE_PLAYER]->addAni(IDB_PLAYERMENU_BUTTON2, "", 1, 429, 74, RGB(255, 255, 255), 0);

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
	{
		if (b)
			bottomDeco[i]->addAni(IDB_STONES, "", 1, 736, 393, RGB(255, 255, 255), 0);
		else
			bottomDeco[i]->addAni(IDB_BUSH, "", 1, 360, 196, RGB(255, 255, 255), 0);
		b = !b;
	}

	for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
		holesChoice[i]->initBackground(IDB_RADIOBTN_BKG, "", 32, 32, false, RGB(255, 255, 255));

	holeNb[0]->addAni(IDB_HOLE_8, "", 1, 24, 36, RGB(255, 255, 255), 0);
	holeNb[1]->addAni(IDB_HOLE_12, "", 1, 46, 36, RGB(255, 255, 255), 0);
}

void PlayerMenu::initPlayerDlg(int rowCount, Str p1, Str p2)
{
	playerDlg = new PlayerDlg(rowCount);

	playerDlg->setSizePolicy(Panel::RESIZE);
	playerDlg->Init();
	playerDlg->initAni();
	playerDlg->Initialize();
	playerDlg->getP1Field()->setName(p1);
	playerDlg->getP2Field()->setName(p2);
	playerDlg->show();
	playerDlg->Refresh();
	playerDlg->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), displayer->GetCurrentMode()->GetHeight());
}

void PlayerMenu::play()
{
}

void PlayerMenu::restoreAni()
{
	int i;

	title->restoreAni();
	palm->restoreAni();
	coconuts->restoreAni();
	backButton->restoreAni();
	holeText->restoreAni();

	if (playerDlg)
	{
		playerDlg->restoreAni();
		playerDlg->Initialize();
		playerDlg->Refresh();
	}

	for (i = 0; i < TOTAL_BUTTON; i++)
		button[i]->restoreAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->restoreAni();

	for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
	{
		holesChoice[i]->restoreAni();
		holeNb[i]->restoreAni();
	}
}

void PlayerMenu::runAnimations()
{
	int i;

	title->runAnimations();
	coconuts->runAnimations();
	backButton->runAnimations();
	palm->runAnimations();

	holeText->runAnimations();

	for (i = 0; i < TOTAL_HOLES_CHOICE; i++)
	{
		holesChoice[i]->runAnimations();
		holeNb[i]->runAnimations();
	}

	for (i = 0; i < TOTAL_BUTTON; i++)
		button[i]->runAnimations();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->runAnimations();

	if (playerDlg && playerDlg->getVisible())
	{
		playerDlg->Run();
		playerDlg->generate(true, cursor);
	}
}
