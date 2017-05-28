#include <csceneslinker.h>
#include "cplayerdlg.h"
#include "cplayermenu.h"
#include "cgamescene.h"
#include "main.h"
#include "../Kares/resource.h"

extern KeyboardListener keybListener;
extern ScenesLinker scenario;

PlayerDlg::PlayerDlg(int rowCount) :
Panel(1, rowCount, 50, 10, 50, false, Text(), Str())
{
	p1Field = new TextField(RGB(0, 0, 0), RGB(255, 255, 255), &keybListener, 24);
	p2Field = new TextField(RGB(0, 0, 0), RGB(255, 255, 255), &keybListener, 24);

	if (RowCount == 8)
	{
		diffRbn[0] = new RadioButton("Facile");
		diffRbn[1] = new RadioButton("Normale");
		diffRbn[2] = new RadioButton("Difficile");
		diffRbn[1]->focus();
	}

	cancelButton = new Visual();

	keybListener.startListenInput();
}

PlayerDlg::~PlayerDlg()
{
	int i;

	delete p1Field;
	delete p2Field;
	delete cancelButton;

	if (RowCount == 8)
		for (i = 0; i < TOTAL_LEVEL; i++)
			delete diffRbn[i];

	keybListener.stopListenInput();
}

bool PlayerDlg::checkFields()
{
	bool b = true;

	if (!p1Field->getName()->GetLength() | !p1Field->isValid())
	{
		p1Field->setInvalid("Entrez un nom pour le joueur 1!");
		b = false;
	}

	if (!p2Field->getName()->GetLength() | !p2Field->isValid())
	{
		p2Field->setInvalid("Entrez un nom pour le joueur 2!");
		b = false;
	}

	return b;
}

void PlayerDlg::destroyAni()
{
	int i;

	Panel::destroyAni();

	p1Field->destroyAni();
	p2Field->destroyAni();

	if (RowCount == 8)
		for (i = 0; i < TOTAL_LEVEL; i++)
			diffRbn[i]->destroyAni();

	GetVisualData(0, RowCount-1)->destroyAni();
	cancelButton->destroyAni();
}

bool PlayerDlg::generate(bool waited, Cursor *cursor)
{
	int i, j;
	PlayerMenu *playerMenu;
	GameScene *gameScene;

	if (ClickOnGrid(0, 1, waited, cursor))
	{
		p1Field->focus();
		p2Field->unfocus();
	}

	if (p1Field->isFocused())
		p1Field->readInput();

	if (ClickOnGrid(0, 3, waited, cursor))
	{
		p2Field->focus();
		p1Field->unfocus();
	}

	if (p2Field->isFocused())
		p2Field->readInput();

	if (RowCount == 8)
		for (i = 0; i < TOTAL_LEVEL; i++)
			if (ClickOnGrid(0, 4+i, waited, cursor))
			{
				diffRbn[i]->focus();
				for (j = 0; j < TOTAL_LEVEL; j++)
					if (j != i)
						diffRbn[j]->unfocus();
				break;
			}

	if (ClickOnGrid(0, RowCount-1, waited, cursor, false))
	{
		if (checkFields())
		{
			gameScene = (GameScene*)scenario.findScene(GAME_SCENE_ID);
			if (gameScene)
			{
				switch (gameScene->getGameMode())
				{
				case GameScene::TWO_PLAYERS:
					gameScene->findPlayer(1)->setName(*p1Field->getName());
					gameScene->findPlayer(2)->setName(*p2Field->getName());
					break;
				case GameScene::FOUR_PLAYERS:
					break;
				}
				switch (gameScene->getPlayerMode())
				{
				case GameScene::ONE_PLAYER_VS_ONE_CPU:
					for (i = 0; i < TOTAL_LEVEL; i++)
						if (diffRbn[i]->isFocused())
						{
							gameScene->setCpuLevel(i+1);
							break;
						}
					break;
				case GameScene::ONE_PLAYER_VS_ONE_PLAYER:
					break;
				}
				playerMenu = (PlayerMenu*)scenario.findScene(PLAYER_MENU_ID);
				if (playerMenu)
				{
					playerMenu->destroyPlayerDlg();
					gameScene->start();
					scenario.goToScene(GAME_SCENE_ID);
					return true;
				}
			}
		}
	}

	if (cursor->buttonPressed() && cursor->collision(cancelButton))
	{
		gameScene = (GameScene*)scenario.findScene(GAME_SCENE_ID);
		if (gameScene)
		{
			gameScene->destroy();
			gameScene->destroyAni();
			scenario.removeScene(gameScene);
			delete gameScene;
		}
		playerMenu = (PlayerMenu*)scenario.findScene(PLAYER_MENU_ID);
		if (playerMenu)
		{
			playerMenu->destroyPlayerDlg();
			return true;
		}
	}
	
	return Panel::generate(waited, cursor);
}

void PlayerDlg::Init()
{
	int i;

	Grid[0][0].data = TEXT;
	Grid[1][0].data = VISUAL_INHERIT;
	Grid[2][0].data = TEXT;
	Grid[3][0].data = VISUAL_INHERIT;

	if (RowCount == 8)
	{
		Grid[4][0].data = VISUAL_INHERIT;
		Grid[5][0].data = VISUAL_INHERIT;
		Grid[6][0].data = VISUAL_INHERIT;
	}

	Grid[RowCount-1][0].data = VISUAL;
	Grid[RowCount-1][0].halign = HALIGN_CENTER;

	Panel::Init();

	*p1Field->getDimension() = Axe(300, 20);
	*p2Field->getDimension() = Axe(300, 20);

	if (RowCount == 8)
		for (i = 0; i < TOTAL_LEVEL; i++)
			*diffRbn[i]->getDimension() = Axe(16, 16);

	*GetVisualData(0, RowCount-1)->getDimension() = Axe(64, 64);
	*cancelButton->getDimension() = Axe(64, 64);

	Grid[1][0].Content = p1Field;
	Grid[3][0].Content = p2Field;

	if (RowCount == 8)
	{
		Grid[4][0].Content = diffRbn[0];
		Grid[5][0].Content = diffRbn[1];
		Grid[6][0].Content = diffRbn[2];
	}

	AlignRows();
	AlignCols();
}

void PlayerDlg::initAni()
{
	int i;

	p1Field->initBackground(IDB_TEXTFIELD_BKG, "", 16, 16, false, -1);
	p2Field->initBackground(IDB_TEXTFIELD_BKG, "", 16, 16, false, -1);

	if (RowCount == 8)
		for (i = 0; i < TOTAL_LEVEL; i++)
			diffRbn[i]->initBackground(IDB_RADIOBTN_BKG, "", 32, 32, false, RGB(255, 255, 255));

	GetVisualData(0, RowCount-1)->addAni(IDB_BTN_OK, "", 1, 159, 134, RGB(255, 255, 255), 0);
	cancelButton->addAni(IDB_BTN_CANCEL, "", 1, 122, 98, RGB(255, 255, 255), 0);

	InitBackground(IDB_PLAYERDLG_BKG, "", 866, 594, false, RGB(255, 255, 255));
}

bool PlayerDlg::Refresh()
{
	int i;

	displayer->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, "Nom du premier joueur:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	displayer->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, "Nom du second joueur:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	p1Field->updateContent();
	p2Field->updateContent();

	if (RowCount == 8)
		for (i = 0; i < TOTAL_LEVEL; i++)
			diffRbn[i]->updateLabel();

	return Panel::Refresh();
}

void PlayerDlg::restoreAni()
{
	int i;

	Panel::restoreAni();

	p1Field->restoreAni();
	p2Field->restoreAni();

	if (RowCount == 8)
		for (i = 0; i < TOTAL_LEVEL; i++)
			diffRbn[i]->restoreAni();

	GetVisualData(0, RowCount-1)->restoreAni();
	cancelButton->restoreAni();
}

bool PlayerDlg::Run(Item *Cursor)
{
	if (Panel::Run(Cursor))
	{
		cancelButton->stickToPanel(Axe(position.x + dimension.x - cancelButton->getDimension()->x - 8, position.y + 8));
		cancelButton->runAnimations();
		return true;
	}

	return false;
}
