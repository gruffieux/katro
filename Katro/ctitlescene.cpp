#include <csceneslinker.h>
#include "ctitlescene.h"
#include "main.h"
#include "../KaRes/resource.h"

extern ScenesLinker scenario;

TitleScene::TitleScene() : Scene(TITLE_SCENE_ID)
{
	title = new Visual();
}

TitleScene::~TitleScene()
{
	delete title;
}

void TitleScene::destroy()
{
}

void TitleScene::destroyAni()
{
	title->destroyAni();
}

void TitleScene::doMouseEvents(Cursor *cursor)
{
	if (cursor->buttonReleased() != Cursor::NO_CLICK)
		scenario.goToNextScene();
}

void TitleScene::init()
{
	*title->getDimension() = Axe(963, 447);

	title->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), displayer->GetCurrentMode()->GetHeight());
}

void TitleScene::initAni()
{
	title->addAni(IDB_TITLE, "", 1, 963, 447, -1, 0);
}

void TitleScene::play()
{
}

void TitleScene::restoreAni()
{
	title->restoreAni();
}

void TitleScene::runAnimations()
{
	title->runAnimations();
}
