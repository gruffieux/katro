#include "cindicator.h"
#include "../KaRes/resource.h"

Indicator::Indicator() :
Panel(0, 0, 0, 0, 41, false, Text(), 0, 0, Str())
{
}

Indicator::Indicator(Str content) :
Panel(0, 0, 0, 0, 41, false, Text(), 0, 0, Str())
{
	name = content;
}

Indicator::~Indicator()
{
}

void Indicator::destroyAni()
{
	text.Destroy();

	Panel::destroyAni();
}

void Indicator::Init()
{
}

void Indicator::initAni()
{
	InitBackground(IDB_INDICATOR_BKG, "", 16, 16, true);
	initBorder(0, IDB_BORDER_TOP, "", 8, 41, RGB(255, 255, 255), true);
	initBorder(1, IDB_BORDER_RIGHT, "", 41, 8, RGB(255, 255, 255), true);
	initBorder(2, IDB_BORDER_BOTTOM, "", 8, 41, RGB(255, 255, 255), true);
	initBorder(3, IDB_BORDER_LEFT, "", 41, 8, RGB(255, 255, 255), true);
	initBorderCorners(IDB_BORDER_CORNERS, "", 41, 41, RGB(255, 255, 255));
}

bool Indicator::Refresh()
{
	updateContent();

	dimension.x = text.GetWidth();

	return Panel::Refresh();
}

void Indicator::restoreAni()
{
	Panel::restoreAni();

	updateContent();
}

void Indicator::runAnimations()
{
	Item area, textArea;

	Run();

	area.getPosition()->x = position.x - BorderSpace;
	area.getPosition()->y = position.y - BorderSpace;
	area.getDimension()->x = dimension.x + 2 * BorderSpace;
	area.getDimension()->y = dimension.y + 2 * BorderSpace;

	textArea.getDimension()->x = text.GetWidth();
	textArea.getDimension()->y = text.GetHeight();
	textArea.centerPosition(&area);

	displayer->Blit(textArea.getPosition()->x, textArea.getPosition()->y, &text, NULL, NULL, DDBLT_WAIT | DDBLT_KEYSRC);
}

void Indicator::updateContent()
{
	displayer->LoadText(&text, NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	text.SetColorKey(RGB(255, 255, 255));
}
